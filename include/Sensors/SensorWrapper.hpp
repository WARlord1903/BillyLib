#ifndef SENSORWRAPPER_HPP
#define SENSORWRAPPER_HPP

#include "main.h"

enum class SensorType {
    ANALOG_OUT,
    ANALOG_IN,
    DIGITAL_OUT,
    DIGITAL_IN,
    BUTTON,
    ENCODER,
    GYRO,
    ULTRASONIC,
    IMU,
    POTENTIOMETER,
    ROTATION,
    DISTANCE,
    OPTICAL,
    End
};

class Sensor{
public:
    Sensor(SensorType t): type{t} {};
    virtual ~Sensor(){};
    SensorType getType() const { return type; }
    virtual std::vector<std::uint8_t> getPorts() = 0;
    virtual std::string toString(){
        std::string ret = "{";
        for(auto& port : getPorts())
            ret += std::to_string(port) + ", ";
        ret = ret.substr(0, ret.length() - 2);
        return ret + "}";
    };
protected:
    SensorType type;
};

#endif