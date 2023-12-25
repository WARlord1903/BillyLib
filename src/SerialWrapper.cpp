#include "main.h"
#include "SerialWrapper.hpp"
#include "Globals.hpp"
#include <string>

void read_serial(void *serial_ptr){
    SerialWrapper* ser = (SerialWrapper*) serial_ptr;
    while(1){
        std::string cmd = ser->getline();
        if(cmd == "GETAUTON"){
            std::ifstream last_auton("/usd/last_auton.txt");
            std::stringstream stream;
            std::string temp;
            while(std::getline(last_auton, temp))
                stream << temp << "\n";
            stream << "END";
            ser->copy_from(stream);
        }
        else if(cmd == "GETPATHS"){
            std::stringstream stream;
            for(auto& path : ini.getVals("Auton Paths"))
                stream << path << "\n";
            stream << "END";
            ser->copy_from(stream);
        }
        else if(cmd.find("GETFILE(") != std::string::npos){
            std::stringstream stream;
            const std::string start = "GETFILE(";
            std::string path = cmd.substr(cmd.find(start) + start.size(), cmd.find_last_of(")") - (cmd.find(start) + start.size()));
            std::string line;
            std::ifstream auton(path);
            while(std::getline(auton, line))
                stream << line << "\n";
            stream << "END";
            ser->copy_from(stream);
        }
        else if(cmd == "GETMOTORS"){
            std::string line;
            std::stringstream stream;
            for(auto& group : motor_map){
                line += group.first + ": {";
                for(auto& port : group.second.getPorts())
                    line += std::to_string(port) + ", ";
                line = line.substr(0, line.size() - 2) + "} {" + 
                       std::to_string(group.second.getGains()[0]) + ", " + std::to_string(group.second.getGains()[1]) + 
                       std::to_string(group.second.getGains()[2]) + "}";
                stream << line << "\n";
                line = "";
            }
            stream << "END";
            ser->copy_from(stream);
        }
        pros::delay(20);
    }
}

std::string SerialWrapper::getline(){ 
    std::string res = ""; 
    char c; 
    while(c != '\n') {
        if(bytes_available() && (c = read()) != '\n') 
            res += c;
        else
            break;
    } 
    return res;
}

void SerialWrapper::copy_from(std::istream& is){
    std::string temp;
    while(std::getline(is, temp)){ 
        if(temp.back() != '\n')
            temp += '\n';
        write(temp);
        pros::delay(5);
    }
}

void SerialWrapper::copy_to(std::ostream& os){
    std::string temp;
    while(!(temp = getline()).empty())
        os << temp << '\n';
}
