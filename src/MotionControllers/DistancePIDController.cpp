#include "MotionControllers/AbstractPIDController.hpp"
#include "main.h"
#include "MotorGroup.hpp"
#include "MotionControllers/DistancePIDController.hpp"

DistancePIDController::DistancePIDController(MotorGroupWrapper& motor): AbstractPIDController(motor.getGains()[0], motor.getGains()[1], motor.getGains()[2], motor) {}



DistancePIDController::DistancePIDController(const DistancePIDController& rhs): AbstractPIDController(
    rhs.motor->getGains()[0],
    rhs.motor->getGains()[1],
    rhs.motor->getGains()[2],
    *(rhs.motor)
) {}


void distancePIDLoop(void *targetPtr){
    double totalError = 0;
    double previousError = 0;
    DistancePIDController *pid = (DistancePIDController*) targetPtr;
    double target = pid->getTarget();
    pid->getMutex().take();
    pid->getMotors().tarePosition();
    while(1){
        if(pros::Task::notify_take(true, 0))
            break;

        double error = target - pid->getMotors().getPosition();
        
        totalError += error;
        double derivative = error - previousError;

        pid->getMotors().moveVelocity(((pid->getkP() * error) + (pid->getkI() * totalError) + (pid->getkD() * derivative)));

        if(std::abs(error) <= 35 && std::abs(derivative) <= 2){
            pros::delay(250);
            break;
        }

        pros::delay(20);

        previousError = error;
    }
    pid->getMotors().moveVelocity(0);
    pid->getMutex().give();
}

