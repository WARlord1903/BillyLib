#include "Globals.hpp"
#include "Sensors/ADIButtonWrapper.hpp"
#include "Sensors/ADIEncoderWrapper.hpp"
#include "Sensors/ADIGyroWrapper.hpp"
#include "Sensors/ADIPotentiometerWrapper.hpp"
#include "Sensors/ADIUltrasonicWrapper.hpp"
#include "Sensors/AnalogIn.hpp"
#include "Sensors/AnalogOut.hpp"
#include "Sensors/DigitalIn.hpp"
#include "Sensors/DigitalOut.hpp"
#include "Sensors/DistanceSensorWrapper.hpp"
#include "Sensors/IMUWrapper.hpp"
#include "Sensors/OpticalSensorWrapper.hpp"
#include "Sensors/RotationSensorWrapper.hpp"
#include "Sensors/SensorWrapper.hpp"
#include "ControllerBindings/ConditionalMacro.hpp"
#include "ini.hpp"

void read_sensors(Ini_file& ini){
    for(auto& s : ini.getKeys("Sensors")){
        std::string data = ini.get("Sensors", s, "");
        std::string ports_str;
        std::string type_str;
        const std::string start = "{";
        const std::string end = "}";
        if(!data.empty()){
            ports_str = data.substr(data.find_first_of(start) + start.size(), data.find_first_of(end) - (data.find_first_of(start) + start.size()));
            data.erase(data.find_first_of(start), data.find_first_of(end) - (data.find_first_of(start) + start.size()) + 2);
            auto remainder = split_by_comma(data.begin(), data.end());
            type_str = remainder.back();
            remainder.pop_back();
            if(type_str == "ANALOG_OUT"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new AnalogOut({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new AnalogOut(std::stoi(remainder[0])));
            }
            else if(type_str == "ANALOG_IN"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new AnalogIn({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new AnalogIn(std::stoi(remainder[0])));
            }
            else if(type_str == "DIGITAL_OUT"){
                if(remainder.size() == 3)
                    sensor_map.emplace(s, (Sensor*)  new DigitalOut({std::stoi(remainder[0]), std::stoi(remainder[1])}, std::stoi(remainder[2])));
                else
                    sensor_map.emplace(s, (Sensor*)  new DigitalOut(std::stoi(remainder[0]), std::stoi(remainder[1])));
            }
            else if(type_str == "DIGITAL_IN"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new DigitalIn({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new DigitalIn(std::stoi(remainder[0])));
            }
            else if(type_str == "BUTTON"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new ADIButtonWrapper({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*) new ADIButtonWrapper(std::stoi(remainder[0])));
            }
            else if(type_str == "ENCODER"){
                if(remainder.size() == 3)
                    sensor_map.emplace(s, (Sensor*)  new ADIEncoderWrapper({std::stoi(remainder[0]), std::stoi(remainder[1]), std::stoi(remainder[2])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new ADIEncoderWrapper({std::stoi(remainder[0]), std::stoi(remainder[1])}));
            }
            else if(type_str == "GYRO"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new ADIGyroWrapper({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new ADIGyroWrapper(std::stoi(remainder[0])));
            }
            else if(type_str == "ULTRASONIC"){
                if(remainder.size() == 3)
                    sensor_map.emplace(s, (Sensor*)  new ADIUltrasonicWrapper({std::stoi(remainder[0]), std::stoi(remainder[1]), std::stoi(remainder[2])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new ADIUltrasonicWrapper({std::stoi(remainder[0]), std::stoi(remainder[1])}));
            }
            else if(type_str == "IMU"){
                if(remainder.back() == "x")
                    sensor_map.emplace(s, (Sensor*)  new IMUWrapper(std::stoi(remainder[0]), IMUAxes::x));
                else if(remainder.back() == "y")
                    sensor_map.emplace(s, (Sensor*)  new IMUWrapper(std::stoi(remainder[0]), IMUAxes::y));
                else
                    sensor_map.emplace(s, (Sensor*)  new IMUWrapper(std::stoi(remainder[0]), IMUAxes::z));
            }
            else if(type_str == "POTENTIOMETER"){
                if(remainder.size() == 2)
                    sensor_map.emplace(s, (Sensor*)  new ADIPotentiometerWrapper({std::stoi(remainder[0]), std::stoi(remainder[1])}));
                else
                    sensor_map.emplace(s, (Sensor*)  new ADIPotentiometerWrapper(std::stoi(remainder[0])));
            }
            else if(type_str == "ROTATION")
                sensor_map.emplace(s, (Sensor*)  new RotationSensorWrapper(std::stoi(remainder[0])));
            else if(type_str == "DISTANCE")
                sensor_map.emplace(s, (Sensor*)  new DistanceSensorWrapper(std::stoi(remainder[0])));
            else if(type_str == "OPTICAL"){
                if(remainder.back() == "saturation")
                    sensor_map.emplace(s, (Sensor*)  new OpticalSensorWrapper(std::stoi(remainder[0]), OpticalSensorOutput::saturation, std::stoi(remainder[1])));
                else if(remainder.back() == "brightness")
                    sensor_map.emplace(s, (Sensor*)  new OpticalSensorWrapper(std::stoi(remainder[0]), OpticalSensorOutput::brightness, std::stoi(remainder[1])));
                else
                    sensor_map.emplace(s, (Sensor*)  new OpticalSensorWrapper(std::stoi(remainder[0]), OpticalSensorOutput::hue, std::stoi(remainder[1])));
            }
        }
    }
}

void write_sensors(Ini_file& ini){
    for(auto& group : sensor_map)
        ini.set("Sensors", group.first, group.second->toString());
}