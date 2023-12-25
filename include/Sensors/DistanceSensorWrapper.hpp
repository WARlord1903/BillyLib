#ifndef DISTANCESENSORWRAPPER_HPP
#define DISTANCESENSORWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class DistanceSensorWrapper : public SinglePortSensor {
public:
    DistanceSensorWrapper(std::uint8_t port): SinglePortSensor{SensorType::DISTANCE, port}, ptr{std::make_unique<DistanceSensor>(port)} {}
    double get() { return ptr->get(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::int32_t getConfidence() { return ptr->getConfidence(); }
    std::int32_t getObjectSize() { return ptr->getObjectSize(); }
    std::int32_t getObjectVelocity() { return ptr->getObjectVelocity(); }
    std::string toString() override { return Sensor::toString() + ", DISTANCE"; }
private:
    std::unique_ptr<DistanceSensor> ptr;
};

#endif