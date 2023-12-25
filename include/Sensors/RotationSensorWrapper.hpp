#ifndef ROTATIONSENSORWRAPPER_HPP
#define ROTATIONSENSORWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class RotationSensorWrapper : public SinglePortSensor {
public:
    RotationSensorWrapper(std::uint8_t port): SinglePortSensor{SensorType::ROTATION, port}, ptr{std::make_unique<RotationSensor>(port)} {}
    double get() { return ptr->get(); }
    std::int32_t reset() { return ptr->reset(); }
    double controllerGet() { return ptr->controllerGet(); }
    double getVelocity() { return ptr->getVelocity(); }
    std::string toString() override { return Sensor::toString() + ", ROTATION"; }
private:
    std::unique_ptr<RotationSensor> ptr;
};

#endif