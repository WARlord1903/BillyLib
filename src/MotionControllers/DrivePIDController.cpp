#include "AbstractPIDController.hpp"
#include "DistancePIDController.hpp"
#include "main.h"
#include "MotionControllers/DrivePIDController.hpp"
void drivePIDLoop(void *targetPtr){
    double totalError = 0;
    double previousError = 0;
    DrivePIDController *pid = (DrivePIDController*) targetPtr;
    pid->getMutex().take();
    double target = std::sqrt(std::pow(pid->getTarget().x, 2) + std::pow(pid->getTarget().y, 2));
    while(1){
        if(pros::Task::notify_take(true, 0))
            break;

        double error = target - std::sqrt(std::pow(pid->getChassis().getX(), 2) + std::pow(pid->getChassis().getY(), 2));

        totalError += error;
        double derivative = error - previousError;

        pid->getChassis().getLeft().moveVelocity((int) ((pid->getkP() * error) + (pid->getkI() * totalError) + (pid->getkD() * derivative)));
        pid->getChassis().getRight().moveVelocity((int) ((pid->getkP() * error) + (pid->getkI() * totalError) + (pid->getkD() * derivative)));

        if(std::abs(error) <= 2 && std::abs(derivative) <= .2){
            pros::delay(250);
            break;
        }

        pros::delay(50);

        previousError = error;
    }
    pid->getChassis().getLeft().moveVelocity(0);
    pid->getChassis().getRight().moveVelocity(0);
    pid->getMutex().give();
}