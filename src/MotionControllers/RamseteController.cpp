#include "MathUtils/MathUtils.hpp"
#include "MathUtils/Matrix.hpp"
#include "MathUtils/Point2D.hpp"
#include "MotionControllers/RamseteController.hpp"

void ramseteLoop(void* rptr){
    RamseteController* r = (RamseteController*) rptr;
    r->getMutex().take();
    r->setFollowing(true);
    int index = 1;
    while(1){
        double error = ptToPtDistance(r->getChassis().getPoint(), {r->getPath()[index][0], r->getPath()[index][1]});
        if(error < 1){
            index++;
            continue;
        }
        Point2D actual = {r->getChassis().getPoint().x * 0.0254, r->getChassis().getPoint().y * 0.0254};
        Point2D desired{r->getPath()[index][0] * 0.0254, r->getPath()[index][1] * 0.0254}; 
        double actual_head = boundRad(r->getChassis().getOrientation() * -1);
        double desired_head = r->getPath()[index][2];
        double desired_lin_vel = r->getPath()[index][3];
        double desired_ang_vel = r->getPath()[index][4];
        Matrix error_m{
            {desired.x - actual.x},
            {desired.y - actual.y},
            {boundRad(desired_head - actual_head)}
        };

        if(error_m[2][0] > M_PI)
            error_m[2][0] -= M_PI * 2;
        else if(error_m[2][0] < -M_PI)
            error_m[2][0] += M_PI * 2;

        Matrix transformation_m {
            {std::cos(actual_head), std::sin(actual_head), 0},
            {-std::sin(actual_head), std::cos(actual_head), 0},
            {0, 0, 1}
        };

        Matrix local_error_m = transformation_m * error_m;

        double k = 2 * r->getPath().getZeta() * std::sqrt(desired_ang_vel * desired_ang_vel + r->getPath().getB() * r->getPath().getB() + desired_lin_vel * desired_lin_vel);

        double v = desired_lin_vel * std::cos(local_error_m[2][0]) + k * local_error_m[0][0];
        double w = desired_ang_vel + k * local_error_m[2][0] + ((r->getPath().getB() * desired_lin_vel * std::sin(local_error_m[2][0]) * local_error_m[1][0]) / local_error_m[2][0]);

        std::cout << v << "\n";

        double linear_vel = (v / (r->getChassis().getDimensions().getWheelDiameter() * 0.0254  * M_PI));

        double angular_vel = (w / (r->getChassis().getDimensions().getWheelDiameter() * 0.0254 * M_PI));

        std::cout << linear_vel << "\n" << angular_vel << "\n\n";

        r->getChassis().getLeft().moveVelocity(linear_vel + angular_vel);
        r->getChassis().getRight().moveVelocity(linear_vel - angular_vel);

        pros::delay(20);
    }
}

void RamseteController::run(RamsetePath &ip){
    p = &ip;
    task = std::make_unique<pros::Task>(ramseteLoop, this, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "RAMSETE Task");
}