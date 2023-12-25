#ifndef OPTICALSENSORWRAPPER_HPP
#define OPTICALSENSORWRAPPER_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"
#include <string>

class OpticalSensorWrapper : public SinglePortSensor {
public:
    OpticalSensorWrapper(std::uint8_t port, OpticalSensorOutput ioutput = OpticalSensorOutput::hue, bool idisableGestures = true): SinglePortSensor{SensorType::OPTICAL, port}, ptr{std::make_unique<OpticalSensor>(port, ioutput, idisableGestures)}, output{ioutput} { if(idisableGestures) disableGestures(); else enableGestures(); }
    double get() { return ptr->get(); }
    double getHue() { return ptr->getHue(); }
    double getBrightness() { return ptr->getBrightness(); }
    double getSaturation() { return ptr->getSaturation(); }
    std::int32_t getLedPWM() { return ptr->getLedPWM(); }
    std::int32_t setLedPWM(std::int8_t ivalue) { return ptr->setLedPWM(ivalue); }
    pros::c::optical_rgb_s_t getRGB() { return ptr->getRGB(); }
    double controllerGet() { return ptr->controllerGet(); }
    std::int32_t enableGestures() { return ptr->enableGestures(); disableGestures_b = false; }
    std::int32_t disableGestures() { return ptr->disableGestures(); disableGestures_b = true; }
    OpticalSensorOutput& getOutputType() { return output; }
    bool gesturesDisabled() { return disableGestures_b; }
    std::string toString() override {
        switch(output){
            case OpticalSensorOutput::hue:
                return Sensor::toString() + ", " + std::to_string(disableGestures_b) + ", hue, OPTICAL";
            case OpticalSensorOutput::saturation:
                return Sensor::toString() + ", " + std::to_string(disableGestures_b) + ", saturation, OPTICAL";
            case OpticalSensorOutput::brightness:
                return Sensor::toString() + ", " + std::to_string(disableGestures_b) + ", brightness, OPTICAL";
            default:
                return "";
        }
    }
private:
    std::unique_ptr<OpticalSensor> ptr;
    OpticalSensorOutput output;
    bool disableGestures_b;
};

#endif