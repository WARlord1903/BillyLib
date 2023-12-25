#include "ControllerBindings/MotorGroupAction.hpp"
#include "Globals.hpp"
#include "ini.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include <sstream>

void read_motor_actions(Ini_file &ini){
    for(auto& s : ini.getKeys("Motor Bindings")){
        std::string actions = ini.get("Motor Bindings", s, "");
        for(auto& action : split_string(actions, ";"))
            motor_controller_map[(ControllerDigital) std::stoi(s)].emplace_back(action);
    }
}

void write_motor_actions(Ini_file &ini){
    for(auto& pair : motor_controller_map){
        std::string txt;
        for(auto& action : pair.second)
            if(!action.toString().empty())
               txt += action.toString() + ";";
        if(!txt.empty())
            ini.set("Motor Bindings", std::to_string((unsigned) pair.first), txt.substr(0, txt.size() - 1));
    }
    ini.save();
}

MotorGroupAction::MotorGroupAction(std::string str): running{false} {
    auto data = split_string(str, ":");
    m = &motor_map[data[0]];
    m_name = data[0];
    voltage = std::stoi(data[1]);
    toggle_state = std::stoi(data[2]);
}

void MotorGroupAction::run(){
    if(!running){
        motor_count[m]++;
        running = true;
    }
    m->moveVoltage(voltage);
}

void MotorGroupAction::toggle(){
    running = !running;
    if(running)
        motor_count[m]++;
    else
        motor_count[m]--;
    m->moveVoltage(voltage * ((running) ? 1 : 0));
}

void MotorGroupAction::stop(){
    if(running){
        running = false;
        motor_count[m]--;
        if(motor_count[m] == 0)
            m->moveVoltage(0);
    }
}

void MotorGroupAction::setName(std::string name){
    m_name = name;
    m = &motor_map[name];
}