#ifndef RAMSETECONTROLLER_HPP
#define RAMSETECONTROLLER_HPP

#include "main.h"
#include "OdomChassis.hpp"
#include "MathUtils/RamsetePath.hpp"

class RamseteController{
public:
    RamseteController(OdomChassis& o): c{&o} {}
    void run(RamsetePath&);
    OdomChassis& getChassis(){ return *c; }
    pros::Mutex& getMutex() { return ramsete_mutex; }
    RamsetePath& getPath() { return *p; }
    void waitUntilSettled() { ramsete_mutex.take(); ramsete_mutex.give(); }
    bool isFollowing() { return following; }
    void setFollowing(bool f) { following = f; }
private:
    OdomChassis* c;
    RamsetePath* p;
    pros::Mutex ramsete_mutex;
    std::unique_ptr<pros::Task> task;
    bool following;
};

#endif