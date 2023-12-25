#include "MotorGroup.hpp"
#include "main.h"
#include "ini.hpp"
#include "Globals.hpp"
#include "types.hpp"
#include <algorithm>
#include <string>

void read_motors(Ini_file& ini){
    for(auto& s : ini.getKeys("Motors")){
        std::string data = ini.get("Motors", s, "");
        std::string ports_str;
        std::string gains_str;
        MotorGroupWrapper null_group;
        const std::string start = "{";
        const std::string end = "}";
        if(!data.empty()){
            ports_str = data.substr(data.find_first_of(start) + start.size(), data.find_first_of(end) - (data.find_first_of(start) + start.size()));
            data.erase(data.find_first_of(start), data.find_first_of(end) - (data.find_first_of(start) + start.size()) + 2);
            gains_str = data.substr(data.find_first_of(start) + start.size(), data.find_first_of(end) - (data.find_first_of(start) + start.size()));
            std::vector<std::string> ports = split_string(ports_str, ", ");
            for(auto& port : ports){
                if(!motor_map.count(s))
                    motor_map[s] = null_group;
                motor_map[s].push_back(std::stoi(port));
            }
            std::vector<std::string> gains = split_string(gains_str, ", ");
            motor_map[s].setGains({std::stod(gains[0]), std::stod(gains[1]), std::stod(gains[2])});
        }
    }
}

void write_motors(Ini_file& ini){
    for(auto& group : motor_map){
        std::string ports;
        std::string gains;
        std::string group_str;
        for(auto& port : group.second.getPorts())
            ports += std::to_string(port) + ", ";
        for(auto& val : group.second.getGains())
            gains += std::to_string(val) + ", ";
        group_str = "{" + ports.substr(0, ports.size() - 2) + "}" +
                    "{" + gains.substr(0, gains.size() - 2) + "}";

        ini.set("Motors", group.first, group_str);
    }
    ini.save();
}