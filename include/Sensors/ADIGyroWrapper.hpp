#ifndef ADIGYROWRAPPER_HPP
#define ADIGYROWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class ADIGyroWrapper : public SinglePortSensor {
public:
    ADIGyroWrapper(std::uint8_t port): SinglePortSensor{SensorType::GYRO, port}, ptr{std::make_unique<ADIGyro>(port)} {}
    ADIGyroWrapper(pros::ext_adi_port_pair_t ports): SinglePortSensor{SensorType::GYRO, ports}, ptr{std::make_unique<ADIGyro>(ports)} {}
    double get() { return ptr->get(); }
    double getRemapped(double iupperBound = 1800, double ilowerBound = -1800) { return ptr->getRemapped(iupperBound, ilowerBound); }
    std::int32_t reset() { return ptr->reset(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::string toString() override { return Sensor::toString() + ", GYRO"; }
private:
    std::unique_ptr<ADIGyro> ptr;
};

#endif