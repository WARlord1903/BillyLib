#include "main.h"
#include "MathUtils/MathUtils.hpp"
#include "MotionControllers/VoltageLog.hpp"

void voltageLogLoop(void *slew){
    VoltageLog *s = (VoltageLog*) slew;
    std::int32_t old_target = s->getTarget();
    std::int32_t local_epoch = pros::millis();
    while(1){
        s->getMotors().moveVoltage(logGenerator(pros::millis() - local_epoch, s->getDuration(), s->getMotors().getVoltage(), s->getTarget()));
        pros::delay(5);
    }
}