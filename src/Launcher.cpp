#include "main.h"
#include "Globals.hpp"
#include "Launcher.hpp"
#include "MathUtils/MathUtils.hpp"
#include "MotionControllers/Slew.hpp"

void launcherLoop(void *launcher){
    Launcher *l = (Launcher*) launcher;
    Slew s(l->getFlywheel());
    while(1){
        // double dist = std::sqrt(std::pow(17.78 - chassis.getX(), 2) + std::pow(17.78 - chassis.getY(), 2)) / 12.0;
        // if(dist < 3.0)
        //     s.run(8000);
        // else if(dist > 16.0)
        //     s.run(12000);
        // else
        //     s.run(calcVelocity(dist));
        // if(pros::Task::notify_take(true, 0)){
        //     if(dist < 3.0)
        //         l->getAngleChanger().set_value(true);
        //     else if(dist > 17.0)
        //         l->getAngleChanger().set_value(false);
        //     else
        //         l->getAngleChanger().set_value(dists[std::round(dist)].second);
        //     pros::delay(200);
        //     l->getIndexer().set_value(true);
        //     pros::delay(200);
        //     l->getIndexer().set_value(false);
        // }
        pros::delay(20);
    }
}