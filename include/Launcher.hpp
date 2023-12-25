#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "main.h"
#include "MotionControllers/Slew.hpp"
#include "MotorGroup.hpp"

void launcherLoop(void*);

class Launcher{
public:
    Launcher(MotorGroupWrapper iflywheel, pros::ADIDigitalOut iangleChanger, pros::ADIDigitalOut iindexer) :flywheel{iflywheel}, angleChanger{std::make_shared<pros::ADIDigitalOut>(iangleChanger)}, indexer{std::make_shared<pros::ADIDigitalOut>(iindexer)}, launcherLoopTask{launcherLoop, this, "Launcher Task"} {}
    Slew& getFlywheel() { return flywheel; }
    pros::ADIDigitalOut& getAngleChanger() { return *angleChanger; }
    pros::ADIDigitalOut& getIndexer() { return *indexer; }
    void run() { launcherLoopTask.resume(); }
    void stop() { launcherLoopTask.suspend();  }
    void launch() { launcherLoopTask.notify(); }
private:
    Slew flywheel;
    std::shared_ptr<pros::ADIDigitalOut> angleChanger;
    std::shared_ptr<pros::ADIDigitalOut> indexer;
    pros::Task launcherLoopTask;
};

#endif