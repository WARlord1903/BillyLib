#ifndef SLEW_HPP
#define SLEW_HPP

#include "main.h"
#include "MotorGroup.hpp"

void slewLoop(void*);

class Slew{
public:
    Slew(MotorGroupWrapper m) :motors{m}, maxChange{0}, stepDuration{0} { motors.setEncoderUnits(AbstractMotor::encoderUnits::degrees); }
    Slew(Slew& s): motors{s.motors}, target{s.target}, maxChange{s.maxChange}, stepDuration{s.stepDuration} {}
    std::int32_t getTarget() const { return target; }
    std::int32_t getMaxChange() const { return maxChange; }
    std::uint32_t getStepDuration() const { return stepDuration; }
    MotorGroupWrapper& getMotors() { return motors; }
    void run(std::int32_t itarget, std::int32_t imaxChange = 1000, std::uint32_t istepDuration = 100) { target = itarget; maxChange = imaxChange; stepDuration = istepDuration; if(!slewRateTask) slewRateTask = std::make_shared<pros::Task>(slewLoop, this, "Slew Task"); slewRateTask->resume(); }
    void stop() { slewRateTask->suspend(); }
private:
    MotorGroupWrapper motors;
    std::int32_t target;
    std::int32_t maxChange;
    std::uint32_t stepDuration;
    std::shared_ptr<pros::Task> slewRateTask;
};

#endif