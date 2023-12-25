#ifndef DIGITALOUT_HPP
#define DIGITALOUT_HPP

#include "main.h"
#include "Sensors/SensorWrapper.hpp"
#include "Sensors/SinglePortSensor.hpp"

class DigitalOut : public SinglePortSensor {
public:
    DigitalOut(std::uint8_t port, bool iinit_state = false): SinglePortSensor{SensorType::DIGITAL_OUT, port}, init_state{iinit_state}, curr_state{iinit_state}, ptr{std::make_unique<pros::ADIDigitalOut>(port, iinit_state)} {}
    DigitalOut(pros::ext_adi_port_pair_t ports, bool iinit_state): SinglePortSensor{SensorType::DIGITAL_OUT, ports}, init_state{iinit_state}, curr_state{iinit_state}, ptr{std::make_unique<pros::ADIDigitalOut>(ports, iinit_state)} {}
    void set_value(bool value) { ptr->set_value(value); curr_state = value; }
    void toggle() { curr_state = !curr_state; ptr->set_value(curr_state); }
    std::string toString() override { return Sensor::toString() + ", " + std::to_string(init_state) + ", DIGITAL_OUT"; }
private:
    std::unique_ptr<pros::ADIDigitalOut> ptr;
    bool init_state;
    bool curr_state;
};

#endif