#ifndef DIGITALIN_HPP
#define DIGITALIN_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class DigitalIn : SinglePortSensor {
public:
    DigitalIn(std::uint8_t port): SinglePortSensor{SensorType::DIGITAL_IN, port}, ptr{std::make_unique<pros::ADIDigitalIn>(port)} {}
    DigitalIn(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::DIGITAL_IN, ports}, ptr{std::make_unique<pros::ADIDigitalIn>(ports)} {}
    std::int32_t get_new_press() { return ptr->get_new_press(); }
    std::int32_t get_value() { return ptr->get_value(); }
    std::string toString() override { return Sensor::toString() + ", DIGITAL_IN"; }
private:
    std::unique_ptr<pros::ADIDigitalIn> ptr;
};

#endif