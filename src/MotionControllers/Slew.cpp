#include "main.h"
#include "MotionControllers/Slew.hpp"

void slewLoop(void *slew){
    Slew *s = (Slew*) slew;
    while(1){
        if(std::abs(s->getTarget() - s->getMotors().getVoltage()) < s->getMaxChange())
            s->getMotors().moveVoltage(s->getTarget());
        else{
            if(s->getMotors().getVoltage() < s->getTarget())
                s->getMotors().moveVoltage(s->getMotors().getVoltage() + s->getMaxChange());
            else
                s->getMotors().moveVoltage(s->getMotors().getVoltage() - s->getMaxChange());
        }
        pros::delay(s->getStepDuration() + 20);
    }
}