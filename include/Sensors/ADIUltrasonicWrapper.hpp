#ifndef ADIULTRASONICWRAPPER_HPP
#define ADIULTRASONICWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/DoublePortSensor.hpp"
#include "okapi/impl/device/adiUltrasonic.hpp"

class ADIUltrasonicWrapper : DoublePortSensor{
public:
    ADIUltrasonicWrapper(pros::ext_adi_port_pair_t ports): DoublePortSensor{SensorType::ULTRASONIC, ports}, ptr{std::make_unique<ADIUltrasonic>(ports.first, ports.second)} {}
    ADIUltrasonicWrapper(pros::ext_adi_port_tuple_t ports): DoublePortSensor{SensorType::ULTRASONIC, ports}, ptr{std::make_unique<ADIUltrasonic>(ports)} {}
    double get() { return ptr->get(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::string toString() override { return Sensor::toString() + ", ULTRASONIC"; }
private:
    std::unique_ptr<ADIUltrasonic> ptr;
};

#endif