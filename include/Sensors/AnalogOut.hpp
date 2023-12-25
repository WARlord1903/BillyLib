#ifndef ANALOGOUT_HPP
#define ANALOGOUT_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class AnalogOut : public SinglePortSensor {
public:
    AnalogOut(std::uint8_t port): SinglePortSensor{SensorType::ANALOG_OUT, port}, ptr{std::make_unique<pros::ADIAnalogOut>(port)} {}
    AnalogOut(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::ANALOG_OUT, ports}, ptr{std::make_unique<pros::ADIAnalogOut>(ports)} {}
    void set_value(std::int32_t value) { ptr->set_value(value); }
    std::string toString() override { return Sensor::toString() + ", ANALOG_OUT"; }
private:
    std::unique_ptr<pros::ADIAnalogOut> ptr;
};

#endif