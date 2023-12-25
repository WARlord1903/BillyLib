#include "MotionControllers/TurnPIDController.hpp"
#include "MathUtils/MathUtils.hpp"

void turnPIDLoop(void *targetPtr){
    double totalError = 0;
    double previousError = 0;
    TurnPIDController *pid = (TurnPIDController*) targetPtr;
    double target = std::atan2(pid->getTarget().y - pid->getChassis().getY(), pid->getTarget().x - pid->getChassis().getX()) * (180 / M_PI) * -1;
    pid->getMutex().take();
    while(1){
        if(pros::Task::notify_take(true, 0))
            break;

        double curr = std::fmod(360 - pid->getChassis().getOrientation() * (180 / M_PI) + 180, 360) - 180;
        if(curr < -180)
            curr += 360;

        double error = findMinAngle(target, curr);

        totalError += error;
        double derivative = error - previousError;

        pid->getChassis().getLeft().moveVelocity((int) ((pid->getkP() * error) + (pid->getkI() * totalError) + (pid->getkD() * derivative)));
        pid->getChassis().getRight().moveVelocity((int) -((pid->getkP() * error) + (pid->getkI() * totalError) + (pid->getkD() * derivative)));

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