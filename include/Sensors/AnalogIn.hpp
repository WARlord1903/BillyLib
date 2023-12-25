#ifndef ANALOGIN_HPP
#define ANALOGIN_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class AnalogIn : public SinglePortSensor {
public:
    AnalogIn(std::uint8_t port): SinglePortSensor{SensorType::ANALOG_IN, port}, ptr{std::make_unique<pros::ADIAnalogIn>(port)} {}
    AnalogIn(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::ANALOG_IN, ports}, ptr{std::make_unique<pros::ADIAnalogIn>(ports)} {}
    std::int32_t calibrate() { return ptr->calibrate(); }
    std::int32_t get_value() { return ptr->get_value(); }
    std::int32_t get_value_calibrated() { return ptr->get_value_calibrated(); }
    std::int32_t get_value_calibrated_HR() { return ptr->get_value_calibrated_HR(); }
    std::string toString() override { return Sensor::toString() + ", ANALOG_IN"; }
private:
    std::unique_ptr<pros::ADIAnalogIn> ptr;
};

#endif