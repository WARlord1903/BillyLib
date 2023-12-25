#ifndef DISTANCEPIDCONTROLLER_HPP
#define DISTANCEPIDCONTROLLER_HPP

#include "main.h"
#include "MotionControllers/AbstractPIDController.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <memory>

void distancePIDLoop(void*);

class DistancePIDController : public AbstractPIDController{
public:
    DistancePIDController(MotorGroupWrapper& motor);
    DistancePIDController(const DistancePIDController&);
    void run(double itarget) override { target = itarget; pid_task = std::make_unique<pros::Task>(distancePIDLoop, this, "Distance PID"); }
    void run(Point2D p) override {}
};

#endif