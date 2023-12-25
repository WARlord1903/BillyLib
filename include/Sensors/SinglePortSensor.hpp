#ifndef SINGLEPORTSENSOR_HPP
#define SINGLEPORTSENSOR_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"

class SinglePortSensor : public Sensor{
public:
    SinglePortSensor(SensorType t, std::uint8_t port): Sensor{t}, ports{port, 0} {}
    SinglePortSensor(SensorType t, pros::ext_adi_port_pair_t iports): Sensor{t}, ports{iports} {}
    std::vector<std::uint8_t> getPorts() override { 
        if(ports.second == 0)
            return {ports.first};
        else
            return {ports.first, ports.second};
    }
protected:
    pros::ext_adi_port_pair_t ports;
};

#endif