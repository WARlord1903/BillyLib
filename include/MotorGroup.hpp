#ifndef MOTORGROUP_HPP
#define MOTORGROUP_HPP

#include "MotionControllers/DistancePIDController.hpp"
#include "ini.hpp"
#include "main.h"
#include <array>

void read_motors(Ini_file&);
void write_motors(Ini_file&);

class MotorGroupWrapper{
public:
    MotorGroupWrapper(): motors{{}} {};
    MotorGroupWrapper(const std::list<int8_t> iports, std::array<double, 3> igains = {0, 0, 0}){ for(auto& port : iports) { motors.emplace_back(port); ports.push_back(port); } gains = igains; tarePosition(); }
    MotorGroupWrapper(const MotorGroupWrapper& rhs) { motors = rhs.motors; ports = rhs.ports; gains = rhs.gains; tarePosition(); }
    MotorGroupWrapper(std::initializer_list<std::int8_t> imotors, std::array<double, 3> igains = {0, 0, 0}) { for(auto& m : imotors) {motors.emplace_back(m); ports.push_back(m); } gains = igains; tarePosition(); }
    void operator=(std::list<int8_t> iports) { motors.clear(); ports.clear(); for(auto& port : iports) { motors.emplace_back(port); ports.push_back(port); } tarePosition(); }
    void operator=(MotorGroupWrapper& rhs) { motors = rhs.motors; ports = rhs.ports; tarePosition(); }
    void moveVoltage(std::int16_t ivoltage) { for (auto& motor : motors) motor.moveVoltage(ivoltage); }
    std::int16_t getVoltage() { return motors.begin()->getVoltage(); }
    void moveVelocity(std::int16_t ivelocity) { for(auto& motor : motors) motor.moveVelocity(ivelocity); }
    std::int32_t getActualVelocity() { return motors.begin()->getActualVelocity(); }
    std::int32_t getTargetVelocity() { return motors.begin()->getTargetVelocity(); }
    void tarePosition() { for(auto& motor : motors) motor.tarePosition(); }
    void moveAbsolute(double iposition, std::int32_t ivelocity) { for(auto& motor : motors) motor.moveAbsolute(iposition, ivelocity); }
    void moveRelative(double iposition, std::int32_t ivelocity) { for(auto& motor : motors) motor.moveRelative(iposition, ivelocity); }
    void setEncoderUnits(AbstractMotor::encoderUnits iunits) { for(auto& motor : motors) motor.setEncoderUnits(iunits); }
    double getPosition() { return motors.begin()->getPosition(); }
    void push_back(std::int8_t port) { motors.emplace_back(port); ports.push_back(port); }
    void setGains(std::array<double, 3> ipgains) { gains = ipgains; }
    std::array<double, 3>& getGains() { return gains; }
    std::list<std::int8_t>& getPorts() { return ports; }
    std::list<Motor>::iterator begin() { return motors.begin(); }
    std::list<Motor>::iterator end() { return motors.end(); }
private:
    std::list<Motor> motors;
    std::list<std::int8_t> ports;
    std::array<double, 3> gains;
    std::shared_ptr<AbstractPIDController> pid;
};

#endif