#include "ControllerBindings/Macro.hpp"
#include "ControllerBindings/ConditionalMacro.hpp"
#include "AutonReader.hpp"
#include "ControllerWrapper.hpp"
#include "Globals.hpp"
#include "MotionControllers/DistancePIDController.hpp"
#include "Sensors/DigitalOut.hpp"

std::ostream& Macro::operator<<(std::ostream &os) const{
    os << "MACRO_DEFINE(" << id << ")\n" << async << "\n" << text << "\n" << "MACRO_END\n";
    return os;
}

std::istream& Macro::operator>>(std::istream& is) {
    text.clear();
    std::string line;
    std::getline(is, line);
    if(std::stoi(line))
        async = true;
    else
        async = false;
    while(std::getline(is, line) && line != "MACRO_END"){
        if(line.back() == '\r')
            line.pop_back();
        text += line + "\n";
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Macro& m){
    return m.operator<<(os);
}

std::istream& operator>>(std::istream& is, Macro& m){
    return m.operator>>(is);
}

void read_macros(std::string path){
    Macro m;
    const std::string definition = "MACRO_DEFINE(";
    std::ifstream ifs(path);
    std::string line;
    while(std::getline(ifs, line)){
        if(line.find(definition) == 0){
            std::string id = line.substr(definition.length(), line.find(")") - definition.length());
            ifs >> m;
            m.setID(id);
            macro_map.emplace(id, m);
        }
    }
    ifs.close();
}

void write_macros(std::string path){
    std::ofstream ofs(path);
    for(auto& pair : macro_map){
        ofs << "MACRO_DEFINE(" << pair.first << ")\n" << pair.second << "\n" << "MACRO_END\n";
    }
    ofs.close();
}

void Macro::run() {
    if(!async)
        mutex.take();
    if(update_time)
        start_time = pros::millis();
    if(update_evals)
        eval_count = 0;
    static std::vector<DistancePIDController> pid_vec = {};
    std::string line;
    std::stringstream ss(text);
    std::string var_str;
    unsigned equal_pos;
    unsigned time_since_pos;
    running = true;
    while(std::getline(ss, line)){
        if(line.back() == '\r')
            line.pop_back();
        strip_string(line);
        while(!(var_str = get_parentheses_group(line, "VAR(")).empty()){
            std::string var_match = "VAR(" + var_str + ")";
            line = line.replace(line.find(var_match), var_match.length(), std::to_string(vars.at(var_str.substr(1, var_str.length() - 2))));    
        }
        if(line.find("PRINT(") == 0){
            for(auto& s : split_by_comma(line.begin() + std::string("PRINT(").length(), line.end() - 1)){
                if(s[0] == '"' && s.back() == '"')
                    std::cout << s.substr(1, s.length() - 2);
                else
                    std::cout << evaluate_value(s, this);
            }
            std::cout << "\n";
        }
        else if(line.find("TURN_TO(") == 0)
            chassis.turnToPoint(get_coords(line));
        else if(line.find("TURN_TO_ASYNC(") == 0)
            chassis.turnToPointAsync(get_coords(line));
        else if(line.find("DRIVE_TO(") == 0)
            chassis.driveToPoint(get_coords(line));
        else if(line.find("DRIVE_TO_ASYNC(") == 0)
            chassis.driveToPointAsync(get_coords(line));
        else if(line.find("TIME_WAIT") == 0 && !(var_str = get_parentheses_group(line, "TIME_WAIT")).empty())
            pros::delay(evaluate_value(var_str, this));
        else if(line.find("CHASSIS_WAIT()") == 0)
            chassis.waitUntilSettled();
        else if(line.find("PP_WAIT()") == 0)
            p_controller.waitUntilSettled();
        else if(line.find("MOVE_DEG") == 0 && !(var_str = get_parentheses_group(line, "MOVE_DEG")).empty()){
            auto vals = split_by_comma(var_str.begin(), var_str.end());
            motor_map[vals[0].substr(1, vals[0].length() - 2)].moveRelative(evaluate_value(vals[1], this), evaluate_value(vals[2], this));
        }
        else if(line.find("MOVE_VOLTAGE") == 0 && !(var_str = get_parentheses_group(line, "MOVE_VOLTAGE")).empty()){
            auto vals = split_by_comma(var_str.begin(), var_str.end());
            motor_map[vals[0].substr(1, vals[0].length() - 2)].moveVoltage(evaluate_value(vals[1], this));
        }
        else if(line.find("MOVE_PID") == 0 && !(var_str = get_parentheses_group(line, "MOVE_PID")).empty()){
            auto vals = split_by_comma(var_str.begin(), var_str.end());
            pid_vec.emplace_back(motor_map[vals[0].substr(1, vals[0].length() - 2)]);
            pid_vec.back().run(evaluate_value(vals[1], this));
            pid_vec.back().waitUntilSettled();
        }
        else if(line.find("MOVE_PID_ASYNC") == 0 && !(var_str = get_parentheses_group(line, "MOVE_PID_ASYNC")).empty()){
            auto vals = split_by_comma(var_str.begin(), var_str.end());
            pid_vec.emplace_back(motor_map[vals[0].substr(1, vals[0].length() - 2)]);
            pid_vec.back().run(evaluate_value(vals[1], this));
        }
        else if(line.find("SET_DIGITAL") == 0 && !(var_str = get_parentheses_group(line, "SET_DIGITAL")).empty()){
            auto vals = split_by_comma(var_str.begin(), var_str.end());
            auto digital = dynamic_cast<DigitalOut*>(sensor_map[vals[0].substr(1, vals[0].length() - 2)].get());
            digital->set_value(evaluate_condition(vals[1], this));
        }
        else if(line.find("IF(") == 0){
            bool else_condition = false;
            ConditionalMacro c;
            ConditionalMacro e;
            std::string temp;
            ss >> c;
            c.setVars(vars);
            c.setStart(start_time);
            c.setEvalCount(eval_count);
            auto pos = ss.tellg();
            std::getline(ss, temp);
            if(temp.find("ELSE{") == 0){
                ss >> e;
                e.setVars(vars);
                e.setStart(start_time);
                e.setEvalCount(eval_count);
                else_condition = true;
            }
            else
                ss.seekg(pos);
            if(evaluate_statement(get_parentheses_group(line, "IF"), this)){
                c.run();
                while(c.isRunning())
                    pros::delay(20);
                auto v = c.getVars();
                for(auto& var : v)
                    if(vars.find(var.first) != vars.end())
                        vars[var.first] = var.second;
                eval_count = c.getEvalCount();
            }
            else if(else_condition){
                e.run();
                while(e.isRunning())
                    pros::delay(20);
                auto v = e.getVars();
                for(auto& var : v)
                    if(vars.find(var.first) != vars.end())
                        vars[var.first] = var.second;
                eval_count = e.getEvalCount();
            }
        }
        else if(line.find("WHILE(") == 0){
            ConditionalMacro c;
            std::string temp;
            ss >> c;
            c.setVars(vars);
            c.setStart(start_time);
            while(evaluate_statement(get_parentheses_group(line, "WHILE"), this)){
                c.run();
            }
            auto v = c.getVars();
            for(auto& var : v)
                if(vars.find(var.first) != vars.end())
                    vars[var.first] = var.second;
        }
        else if(line.find("RUN_MACRO(") == 0)
            macro_map[get_parentheses_group(line, "RUN_MACRO")].run();
        else if(get_non_quoted_indices(line, '=').size() == 1 && (equal_pos = get_non_quoted_indices(line, '=')[0])){
            if(equal_pos >= 1 && equal_pos < line.length())
                vars[line.substr(0, equal_pos)] = evaluate_value(line.substr(equal_pos + 1), this);
        }
    }
    running = false;
    mutex.give();
}