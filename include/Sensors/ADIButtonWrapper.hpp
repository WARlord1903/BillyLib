#ifndef ADIBUTTONWRAPPER_HPP
#define ADIBUTTONWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class ADIButtonWrapper : public SinglePortSensor {
public:
    ADIButtonWrapper(std::uint8_t port): SinglePortSensor{SensorType::BUTTON, port}, ptr{std::make_unique<ADIButton>(port)} {}
    ADIButtonWrapper(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::BUTTON, ports}, ptr{std::make_unique<ADIButton>(ports)} {}
    bool isPressed() { return ptr->isPressed(); }
    bool changed() { return ptr->changed(); }
    bool changedToPressed() { return ptr->changedToPressed(); }
    bool changedToReleased() { return ptr->changedToReleased(); }
    std::string toString() override { return Sensor::toString() + ", BUTTON"; }
private:
    std::unique_ptr<ADIButton> ptr;
};

#endif