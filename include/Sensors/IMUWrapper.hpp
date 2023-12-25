#ifndef IMUWRAPPER_HPP
#define IMUWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class IMUWrapper : public SinglePortSensor{
public:
    IMUWrapper(std::uint8_t port, IMUAxes iaxis): SinglePortSensor{SensorType::IMU, port}, ptr{std::make_unique<IMU>(port, iaxis)}, axis{iaxis} {}
    double get() { return ptr->get(); }
    double getRemapped(double iupperBound = 1800, double ilowerBound = -1800) { return ptr->getRemapped(iupperBound, ilowerBound); }
    double getAcceleration() { return ptr->getAcceleration(); }
    std::int32_t reset() { return ptr->reset(); }
    std::int32_t reset(double inewAngle) { return ptr->reset(inewAngle); }
    std::int32_t calibrate() { return ptr->calibrate(); }
    double controllerGet() { return ptr->controllerGet(); }
    bool isCalibrating() { return ptr->isCalibrating(); }
    IMUAxes& getAxis() { return axis; }
    void setAxis(IMUAxes iaxis) { ptr = std::make_unique<IMU>(ports.first, iaxis); }
    std::string toString() override {
        switch(axis){
            case IMUAxes::x:
                return Sensor::toString() + ", x, IMU"; 
            case IMUAxes::y:
                return Sensor::toString() + ", y, IMU"; 
            case IMUAxes::z:
                return Sensor::toString() + ", z, IMU"; 
            default:
                return "";
        }
    }
private:
    std::unique_ptr<IMU> ptr;
    IMUAxes axis;
};

#endif