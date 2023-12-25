#ifndef ADIPOTENTIOMETERWRAPPER_HPP
#define ADIPOTENTIOMETERWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class ADIPotentiometerWrapper : public SinglePortSensor {
public:
    ADIPotentiometerWrapper(std::uint8_t port): SinglePortSensor{SensorType::POTENTIOMETER, port}, ptr{std::make_unique<Potentiometer>(port)} {}
    ADIPotentiometerWrapper(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::POTENTIOMETER, ports}, ptr{std::make_unique<Potentiometer>(ports)} {}
    double get() { return ptr->get(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::string toString() override { return Sensor::toString() + ", POTENTIOMETER"; }
private:
    std::unique_ptr<Potentiometer> ptr;
};

#endif