#ifndef ADIENCODERWRAPPER_HPP
#define ADIENCODERWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/DoublePortSensor.hpp"

class ADIEncoderWrapper : public DoublePortSensor {
public:
    ADIEncoderWrapper(pros::ext_adi_port_pair_t ports): DoublePortSensor{SensorType::ENCODER, ports}, ptr{std::make_unique<ADIEncoder>(ports.first, ports.second)} {}
    ADIEncoderWrapper(pros::ext_adi_port_tuple_t ports): DoublePortSensor{SensorType::ENCODER, ports}, ptr{std::make_unique<ADIEncoder>(ports)} {}
    double get() { return ptr->get(); }
    std::int32_t reset() { return ptr->reset(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::string toString() override { return Sensor::toString() + ", ENCODER"; }
private:
    std::unique_ptr<ADIEncoder> ptr;
};

#endif