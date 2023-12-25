#include "main.h"
#include "AutonReader.hpp"
#include "Globals.hpp"
#include "MathUtils/Point2D.hpp"
#include "pros/rtos.hpp"
#include <string>
#include <variant>

Point2D get_coords(std::string line){
    std::size_t first = line.find('(');
    std::size_t len = line.find(')') - first - 1;
    return strToPoint(line.substr(first + 1, len));
}

std::uint32_t get_delay_time(std::string line){
    std::size_t first = line.find('(');
    std::size_t len = line.find(')') - first - 1;
    return std::stoi(line.substr(first + 1, len));
}

std::pair<std::string, double> get_motor_group_pid(std::string cmd){
    std::string begin = "MOVE_PID(\"";
    std::string end = "\", ";
    std::string type = cmd.substr(cmd.find(begin) + begin.length(), cmd.find(end) - (cmd.find(begin) + begin.length()));
    cmd.erase(0, cmd.find(end) + end.length());
    return {type, std::stod(cmd.substr(0, cmd.find(end)))};
}

std::pair<std::string, std::int16_t> get_motor_group_voltage(std::string cmd){
    std::string begin = "MOVE_VOLTAGE(\"";
    std::string end = "\", ";
    std::string type = cmd.substr(cmd.find(begin) + begin.length(), cmd.find(end) - (cmd.find(begin) + begin.length()));
    cmd.erase(0, cmd.find(end) + end.length());
    end = ")";
    return {type, std::stoi(cmd.substr(0, cmd.find(end)))};
}

std::tuple<std::string, double, std::int32_t> get_motor_group_position(std::string cmd){
    std::string begin = "MOVE_DEG(\"";
    std::string end = "\", ";
    std::string type = cmd.substr(cmd.find(begin) + begin.length(), (cmd.find(end) - cmd.find(begin) - begin.length()));
    cmd.erase(0, cmd.find(end) + end.length());
    end = ", ";
    std::string position = cmd.substr(0, (cmd.find(end)));
    cmd.erase(0, cmd.find(end) + end.length());
    end = ")";
    return {type, std::stod(position), std::stoi(cmd.substr(0, (cmd.find(end) - cmd.find(begin) - begin.length())))};
}

std::tuple<double, double, double> getPos(std::string cmd){
    std::size_t first = cmd.find("(");
    std::size_t len = cmd.find(", ");
    std::string x = cmd.substr(first + 1, len - first - 1);
    cmd = cmd.erase(0, len + 2);
    len = cmd.find(", ");
    std::string y = cmd.substr(0, len);
    cmd = cmd.erase(0, len + 2);
    return {std::stod(x), std::stod(y), std::stod(cmd.substr(0, cmd.find(")")))};
}

void run_auton(void *auton_reader_ptr){
    AutonReader* reader = (AutonReader*) auton_reader_ptr;
    while(1){
        if(pros::Task::notify_take(true, 0)){
            if(std::holds_alternative<std::string>(reader->getCommand())){
                std::string cmd = std::get<std::string>(reader->getCommand());
                if(cmd.find("STARTING_POS(") != std::string::npos){
                    auto pos = getPos(cmd);
                    chassis.setPos(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
                }
                else if(cmd.find("TURN_TO(") != std::string::npos)
                    chassis.turnToPoint(get_coords(cmd));
                else if(cmd.find("TURN_TO_ASYNC(") != std::string::npos)
                    chassis.turnToPointAsync(get_coords(cmd));
                else if(cmd.find("DRIVE_TO(") != std::string::npos)
                    chassis.driveToPoint(get_coords(cmd));
                else if(cmd.find("DRIVE_TO_ASYNC(") != std::string::npos)
                    chassis.driveToPointAsync(get_coords(cmd));
                else if(cmd.find("CHASSIS_WAIT(") != std::string::npos)
                    chassis.waitUntilSettled();
                else if(cmd.find("PP_WAIT(") != std::string::npos)
                    p_controller.waitUntilSettled();
                else if(cmd.find("TIME_WAIT("))
                    pros::delay(get_delay_time(cmd));
                else if(cmd.find("MOVE_DEG(") != std::string::npos){
                    auto data = get_motor_group_position(cmd);
                    motor_map.at(std::get<0>(data)).moveRelative(std::get<1>(data), std::get<2>(data));
                }
                else if(cmd.find("MOVE_VOLTAGE(") != std::string::npos)
                    motor_map.at(get_motor_group_voltage(cmd).first).moveVoltage(get_motor_group_voltage(cmd).second);
            }
            else{
                p_controller.run(*std::get<PurePursuitPath*>(reader->getCommand()), *reader->getDistanceSensor());
                p_controller.waitUntilSettled();
            }
        }
        pros::delay(20);
    }
}