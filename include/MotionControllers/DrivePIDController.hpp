#ifndef DRIVEPIDCONTROLLER_HPP
#define DRIVEPIDCONTROLLER_HPP

#include "main.h"
#include "MotionControllers/AbstractPIDController.hpp"
#include "OdomChassis.hpp"

void drivePIDLoop(void*);

class DrivePIDController : public AbstractPIDController{
public:
    DrivePIDController(double ikP, double ikI, double ikD, OdomChassis& ic): AbstractPIDController(ikP, ikI, ikD), c{ic} {}
    void run(double t) override {}
    void run(Point2D itarget) override { target = itarget; pid_task = std::make_unique<pros::Task>(drivePIDLoop, this, "Drive PID"); }
    OdomChassis& getChassis() { return c; }
    Point2D& getTarget() { return target; }
    bool getReverse() { return rev; }
private:
    Point2D target;
    bool rev;
    OdomChassis c;
};

#endif