#ifndef VOLTAGELOG_HPP
#define VOLTAGELOG_HPP

#include "MotorGroup.hpp"
#include "main.h"
#include "types.hpp"

void voltageLogLoop(void*);

class VoltageLog{
public:
    VoltageLog(MotorGroupWrapper imotors, std::int32_t itarget, std::uint32_t iduration = 2000) :motors{imotors}, target{itarget}, duration{iduration} { motors.setEncoderUnits(AbstractMotor::encoderUnits::degrees); }
    MotorGroupWrapper& getMotors() { return motors; }
    std::int32_t getTarget() const { return target; }
    std::uint32_t getDuration() const { return duration; }
    void setDuration(std::uint32_t iduration) { duration = iduration; }
    void setTarget(std::int32_t itarget) { target = itarget; }
    void run() { slewRateTask = std::make_unique<pros::Task>(voltageLogLoop, this, "Voltage Logarithm"); }
private:
    MotorGroupWrapper motors;
    std::int32_t target;
    std::uint32_t duration;
    std::unique_ptr<pros::Task> slewRateTask;
};

#endif