#ifndef DOUBLEPORTSENSOR_HPP
#define DOUBLEPORTSENSOR_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"

class DoublePortSensor : public Sensor{
public:
    DoublePortSensor(SensorType t, pros::ext_adi_port_pair_t iports): Sensor{t}, ports{iports.first, iports.second, 0} {}
    DoublePortSensor(SensorType t, pros::ext_adi_port_tuple_t iports): Sensor{t}, ports{iports} {}
    std::vector<std::uint8_t> getPorts() {
        if(std::get<2>(ports) == 0)
            return {std::get<0>(ports), std::get<1>(ports)};
        else
            return {std::get<0>(ports), std::get<1>(ports), std::get<2>(ports)};
    }
protected:
    pros::ext_adi_port_tuple_t ports;
};

#endif