#ifndef ABSTRACTPIDCONTROLLER_HPP
#define ABSTRACTPIDCONTROLLER_HPP

#include "main.h"
#include "types.hpp"
#include "MathUtils/Point2D.hpp"
#include <memory>

class MotorGroupWrapper;

class AbstractPIDController{
public:
    virtual ~AbstractPIDController() = default;
    MotorGroupWrapper& getMotors() { return *motor; }
    std::tuple<double, double, double> getGains() { return {kP, kI, kD}; }
    double getTarget() { return target; }
    double getkP() { return kP; }
    double getkI() { return kI; }
    double getkD() { return kD; }
    pros::Mutex& getMutex() { return pidMutex; }
    void waitUntilSettled() { pidMutex.take(); pidMutex.give(); }
    virtual void run(double) = 0;
    virtual void run(Point2D) = 0;
protected:
    AbstractPIDController(double ikP, double ikI, double ikD): kP{ikP}, kI{ikI}, kD{ikD} {}
    AbstractPIDController(double ikP, double ikI, double ikD, MotorGroupWrapper& imotor): motor{&imotor}, kP{ikP}, kI{ikI}, kD{ikD} { }
    MotorGroupWrapper* motor;
    double target;
    double kP;
    double kI;
    double kD;
    std::unique_ptr<pros::Task> pid_task;
    pros::Mutex pidMutex;
};

#endif