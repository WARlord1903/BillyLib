#include "ControllerBindings/ConditionalMacro.hpp"
#include "Globals.hpp"
#include "Macro.hpp"
#include "MathUtils/Point2D.hpp"
#include "Sensors/DigitalOut.hpp"
#include "Sensors/IMUWrapper.hpp"
#include "ini.hpp"
#include <cmath>

std::vector<std::string> split_by_comma(const std::string::iterator& b, const std::string::iterator& e){
    std::string val(b, e);
    std::vector<std::string> ret;
    bool escape = false;
    bool in_quote = false;
    unsigned parentheses_count = 0;
    std::vector<unsigned> split_indices;
    unsigned curr_index = 0;
    std::for_each(b, e, [&](const char c){
        if(c == '\\')
            escape = true;
        else
            escape = false;
        if(c == '"' && !in_quote)
            in_quote = true;
        else if(c == '"' && in_quote && !escape)
            in_quote = false;
        if(c == '(' && !in_quote)
            parentheses_count++;
        else if(c == ')' && !in_quote)
            parentheses_count--;
        if(c == ',' && !in_quote && parentheses_count == 0)
            split_indices.push_back(curr_index);
        curr_index++;
    });
    split_indices.push_back(val.length());
    unsigned last_index = 0;
    for(int i = 0; i < split_indices.size(); i++){
        ret.push_back(strip_string(val.substr(last_index, split_indices[i] - last_index)));
        last_index = split_indices[i] + 1;
    }
    return ret;
}

std::string strip_string(std::string s){
    while(std::isspace(s[0]))
        s = s.substr(1);
    while(std::isspace(s.back()))
        s.pop_back();
    return s;
}

unsigned get_occurances(std::string s, std::string sub){
    unsigned num_expressions = 0;
    std::string::size_type pos = 0;
    while((pos = s.find(sub, pos)) != std::string::npos){
        num_expressions++;
        pos += std::string(sub).length();
    }
    return num_expressions;
}

std::string pad_parentheses(std::string s){
    int unclosed_count = get_occurances(s, "(") - get_occurances(s, ")");
    for(int i = 0; i < unclosed_count; i++)
        s += ")";
    for(int i = 0; i > unclosed_count; i--)
        s.insert(s.begin(), '(');
    if(s[0] == '(' && s.back() == ')')
        s = s.substr(1, s.length() - 2);
    return s;
}

std::vector<unsigned> get_non_quoted_indices(std::string s, char search, bool reverse){
    bool in_quotes = false;
    bool escape = false;
    std::vector <unsigned> ret;
    if(!reverse){
        for(int i = 0; i < s.length(); i++){
            if(s[i] == '\\' && !escape)
                escape = true;
            else
                escape = false;
            if(s[i] == '"' && !escape)
                in_quotes = !in_quotes;
            else if(s[i] == search && !in_quotes)
                ret.push_back(i);
        }
    }
    else{
        for(int i = s.length() - 1; i >= 0; i--){
            if(s[i] == '"'){
                if(i > 0 && s[i-1] != '\\')
                    in_quotes = !in_quotes;
            }
            else if(s[i] == search && !in_quotes)
                ret.push_back(i);
        }   
    }
    return ret;
}

std::string get_parentheses_group(std::string s, std::string start){
    if(start.back() != '(')
        start.push_back('(');
    auto pos = s.find(start);
    if(pos == std::string::npos)
        return "";
    std::string orig = s;
    auto open_indices = get_non_quoted_indices(s, '(');
    auto close_indices = get_non_quoted_indices(s, ')', true);
    unsigned open_index;
    unsigned close_index;
    for(int i = 0; i < open_indices.size(); i++){
        if(open_indices[i] >= pos && open_indices[i] < pos + start.length()){
            open_index = open_indices[i];
            close_index = close_indices[i];
            break;
        }
    }
    s = s.substr(open_index + 1, close_index - open_index - 1);
    while((open_index = s.find('(')) > (close_index = s.find(')'))){
        if(open_index == std::string::npos || close_index == std::string::npos)
            break;
        while(s.back() != ')')
            s.pop_back();
        s.pop_back();
    }
    if(s.find('(') == std::string::npos && s.find(')') != std::string::npos)
        while(s.find(")") != std::string::npos)
            s.pop_back();
    return s;
}

double evaluate_expression(std::string expr_s, Macro* m_ptr = nullptr){
    std::string expr = expr_s;
    std::vector<double> vals;
    std::list<std::pair<char, unsigned> > operators;
    double res;
    unsigned num_expressions = 0;
    std::string::size_type pos = 0;
    std::string val;
    while(!(val = get_parentheses_group(expr, "EXPR(")).empty()){
        std::string match = "EXPR(" + val + ")";
        expr = expr.replace(expr.find(match), match.length(), std::to_string(evaluate_expression(val, m_ptr)));
    }
    unsigned val_index = 1;
    unsigned char_index = 0;
    unsigned last_operator_index = 0;
    std::for_each(expr.begin(), expr.end(), [&](const char c){
        char_index++;
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^'){
            operators.push_back({c, val_index++});
            if(vals.empty())
                vals.push_back(evaluate_value(strip_string(expr.substr(0, char_index - 1)), m_ptr));
            else
                vals.push_back(evaluate_value(strip_string(expr.substr(last_operator_index, char_index - last_operator_index - 1)), m_ptr));
            last_operator_index = char_index;
        }
    });
    vals.push_back(evaluate_value(strip_string(expr.substr(last_operator_index)), m_ptr));
    if(operators.size() > 0 && vals.size() == operators.size() + 1){
        std::map<char, unsigned> precedence = {
            {'^', 2},
            {'*', 1},
            {'/', 1},
            {'%', 1},
            {'+', 0},
            {'-', 0},
        };
        operators.sort([&](std::pair<char, unsigned>& a, std::pair<char, unsigned>& b) {
            return precedence[a.first] >= precedence[b.first] && a.second < b.second;
        });
        res = vals[operators.begin()->second - 1];
        for(auto& p : operators){
            switch(p.first){
                case '^':
                    res = std::pow(res, vals[p.second]);
                    break;
                case '+':
                    res += vals[p.second];
                    break;
                case '-':
                    res -= vals[p.second];
                    break;
                case '*':
                    res *= vals[p.second];
                    break;
                case '/':
                    res /= vals[p.second];
                    break;
                case '%':
                    res = std::fmod(res, vals[p.second]);
                    break;
            }
        }
        return res;
    }
    else if(vals.size() == 1)
        return vals[0];
    return 0;
}

double evaluate_value(std::string condition, Macro* m_ptr){
    std::string val;
    if(m_ptr)
        m_ptr->incrementEvalCount();
    if(condition[0] == '-')
        condition = "NEGATIVE(" + condition.substr(1) + ")";
    if(condition.find("EXPR") == 0 && !(val = get_parentheses_group(condition, "EXPR")).empty())
        return evaluate_expression(val, m_ptr);
    else if(condition.find("MOTOR_VOLTAGE") == 0 && !(val = get_parentheses_group(condition, "MOTOR_VOLTAGE(")).empty())
        return motor_map[val.substr(1, val.length() - 2)].getVoltage();
    else if(condition.find("MOTOR_POSITION") == 0 && !(val = get_parentheses_group(condition, "MOTOR_POSITION(")).empty())
        return motor_map[val.substr(1, val.length() - 2)].getPosition();
    else if(condition.find("DISTANCE_FROM_POINT") == 0 && !(val = get_parentheses_group(condition, "DISTANCE_FROM_POINT(")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return ptToPtDistance(chassis.getPoint(), {std::stod(vals[0]), std::stod(vals[1])});
    }
    else if(condition.find("CHASSIS_X()") == 0)
        return chassis.getX();
    else if(condition.find("CHASSIS_Y()") == 0)
        return chassis.getY();
    else if(condition.find("CHASSIS_HEADING()") == 0)
        return chassis.getOrientation();
    else if(condition.find("NEGATIVE(") == 0 && !(val = get_parentheses_group(condition, "NEGATIVE(")).empty())
        return -evaluate_value(val, m_ptr);
    else if(condition.find("ABS(") == 0 && !(val = get_parentheses_group(condition, "ABS")).empty())
        return std::abs(evaluate_value(val, m_ptr));
    else if(condition.find("SIN(") == 0 && !(val = get_parentheses_group(condition, "SIN(")).empty())
        return std::sin(evaluate_value(val, m_ptr));
    else if(condition.find("COS(") == 0 && !(val = get_parentheses_group(condition, "COS(")).empty())
        return std::cos(evaluate_value(val, m_ptr));
    else if(condition.find("TAN(") == 0 && !(val = get_parentheses_group(condition, "TAN(")).empty())
        return std::tan(evaluate_value(val, m_ptr));
    else if(condition.find("FLOOR(") == 0 && !(val = get_parentheses_group(condition, "FLOOR(")).empty())
        return std::floor(evaluate_value(val, m_ptr));
    else if(condition.find("CEIL(") == 0 && !(val = get_parentheses_group(condition, "CEIL(")).empty())
        return std::ceil(evaluate_value(val, m_ptr));
    else if(condition.find("ROUND(") == 0 && !(val = get_parentheses_group(condition, "ROUND(")).empty())
        return std::round(evaluate_value(val, m_ptr));
    else if(condition.find("ATAN2(") == 0 && !(val = get_parentheses_group(condition, "ATAN2(")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());        
        return std::atan2(evaluate_value(vals[0], m_ptr), evaluate_value(vals[1], m_ptr));
    }
    else if(condition.find("ROOT(") == 0 && !(val = get_parentheses_group(condition, "ROOT(")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());        
        return std::pow(evaluate_value(vals[0], m_ptr), 1. / evaluate_value(vals[1], m_ptr));
    }
    else if(condition.find("GET_IMU(") == 0 && !(val = get_parentheses_group(condition, "GET_IMU(")).empty()){
        auto imu = dynamic_cast<IMUWrapper*>(sensor_map[val.substr(1, val.length() - 2)].get());
        return imu->get();
    }
    else if(condition.find("GET_IMU_REMAPPED(") == 0 && !(val = get_parentheses_group(condition, "GET_IMU(")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        auto imu = dynamic_cast<IMUWrapper*>(sensor_map[val.substr(1, val.length() - 2)].get());
        return imu->getRemapped(evaluate_value(vals[1]), evaluate_value(vals[2]));
    }
    else if(condition.find("TIME_SINCE_START()") == 0)
        return pros::millis() - control_start;
    else if(condition.find("TIME_SINCE_EXECUTION()") == 0){
        if(m_ptr)
            return pros::millis() - m_ptr->getStartTime();
        else
            return 0;
    }
    else if(condition.find("EVAL_COUNT()") == 0){
        if(m_ptr)
            return m_ptr->getEvalCount();
        else
            return 0;
    }
   return std::stod(condition);
}

bool get_controller_press(std::string condition){
    if(condition == "L1")
        return controller[ControllerDigital::L1].isPressed();
    if(condition == "L2")
        return controller[ControllerDigital::L2].isPressed();
    if(condition == "R1")
        return controller[ControllerDigital::R1].isPressed();
    if(condition == "R2")
        return controller[ControllerDigital::R2].isPressed();
    if(condition == "UP")
        return controller[ControllerDigital::up].isPressed();
    if(condition == "DOWN")
        return controller[ControllerDigital::down].isPressed();
    if(condition == "LEFT")
        return controller[ControllerDigital::left].isPressed();
    if(condition == "RIGHT")
        return controller[ControllerDigital::right].isPressed();
    if(condition == "A")
        return controller[ControllerDigital::A].isPressed();
    if(condition == "B")
        return controller[ControllerDigital::B].isPressed();
    if(condition == "X")
        return controller[ControllerDigital::X].isPressed();
    if(condition == "Y")
        return controller[ControllerDigital::Y].isPressed();
    else
        return false;
}

bool evaluate_condition(std::string condition, Macro* m_ptr){
    if(m_ptr)
        m_ptr->incrementEvalCount();
    std::string val;
    if(condition.find("LESS_THAN") == 0 && !(val = get_parentheses_group(condition, "LESS_THAN")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) < evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("LESS_THAN_OR_EQUAL") == 0 && !(val = get_parentheses_group(condition, "LESS_THAN_OR_EQUAL")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) <= evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("GREATER_THAN") == 0 && !(val = get_parentheses_group(condition, "GREATER_THAN")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) > evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("GREATER_THAN_OR_EQUAL") == 0 && !(val = get_parentheses_group(condition, "GREATER_THAN_OR_EQUAL")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) >= evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("EQUAL") == 0 && !(val = get_parentheses_group(condition, "EQUAL")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) == evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("NOT_EQUAL") == 0 && !(val = get_parentheses_group(condition, "NOT_EQUAL")).empty()){
        auto vals = split_by_comma(val.begin(), val.end());
        return evaluate_value(vals[0], m_ptr) != evaluate_value(vals[1], m_ptr);
    }
    else if(condition.find("NOT") == 0 && !(val = get_parentheses_group(condition, "NOT")).empty())
        return !evaluate_condition(val, m_ptr);
    else if(condition.find("CONTROLLER_PRESSED") == 0 && !(val = get_parentheses_group(condition, "CONTROLLER_PRESSED")).empty())
        return get_controller_press(val);
    else if(condition.find("TRUE()") == 0)
        return true;
    else if(condition.find("FALSE()") == 0)
        return false;
    else if(condition.find("GROUP") == 0 && !(val = get_parentheses_group(condition, "CONTROLLER_PRESSED")).empty())
        return evaluate_statement(val, m_ptr);
    return false;
}

bool evaluate_statement(std::string condition_s, Macro* m_ptr){
    std::string condition = condition_s;
    std::string val;
    std::vector<bool> vals;
    std::list<std::pair<char, unsigned> > operators;
    bool truth;
    while(!(val = get_parentheses_group(condition, "GROUP(")).empty()){
        std::string match = "GROUP(" + val + ")";
        if(evaluate_condition(val, m_ptr))
            condition = condition.replace(condition.find(match), match.length(), "TRUE()");
        else
            condition = condition.replace(condition.find(match), match.length(), "FALSE()");
    }
    unsigned val_index = 1;
    unsigned char_index = 0;
    unsigned last_operator_index = 0;
    std::for_each(condition.begin(), condition.end(), [&](const char c){
        char_index++;
        if(c == '&' || c == '|'){
            operators.push_back({c, val_index++});
            if(vals.empty())
                vals.push_back(evaluate_condition(strip_string(condition.substr(0, char_index - 1)), m_ptr));
            else
                vals.push_back(evaluate_condition(strip_string(condition.substr(last_operator_index, char_index - last_operator_index - 1)), m_ptr));
            last_operator_index = char_index;
        }
    });
    vals.push_back(evaluate_condition(strip_string(condition), m_ptr));
    if(vals.size() > 1 && operators.size() > 0){
        std::map<char, unsigned> precedence = {
            {'&', 1},
            {'|', 0}
        };
        operators.sort([&](std::pair<char, unsigned>& a, std::pair<char, unsigned>& b){
            return precedence[a.first] >= precedence[b.first] && a.second > b.second;
        });
        truth = vals[operators.begin()->second - 1];
        for(auto& p : operators){
            if(p.first == '&')
                truth = truth && vals[p.second];
            else
                truth = truth || vals[p.second];
        }
        return truth;
    }
    else if(vals.size() == 1)
        return vals[0];
    return false;
}

std::istream& ConditionalMacro::operator>>(std::istream& is){
    text.clear();
    async = false;
    nested_contitionals = 0;
    std::string line;
    while(std::getline(is, line)){
        if(line.back() == '\r')
            line.pop_back();
        if(line.back() == '{')
            nested_contitionals++;
        else if(line.back() == '}'){
            if(nested_contitionals > 0)
                nested_contitionals--;
            else
                break;
        }
        text += line + "\n";
    }
    return is;
}