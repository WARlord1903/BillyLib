#ifndef TURNPIDCONTROLLER_HPP
#define TURNPIDCONTROLLER_HPP

#include "main.h"
#include "OdomChassis.hpp"
#include "MotionControllers/AbstractPIDController.hpp"
#include "pros/rtos.h"
#include <memory>

void turnPIDLoop(void*);

class TurnPIDController : public AbstractPIDController{
public:
    TurnPIDController(double ikP, double ikI, double ikD, OdomChassis& ic): AbstractPIDController(ikP, ikI, ikD), c{ic} {}
    void run(double t) override {}
    void run(Point2D itarget) override { target = itarget; pid_task = std::make_unique<pros::Task>(turnPIDLoop, this, "Turn PID"); }
    OdomChassis& getChassis() { return c; }
    Point2D& getTarget() { return target; }
private:
    Point2D target;
    OdomChassis c;
};

#endif