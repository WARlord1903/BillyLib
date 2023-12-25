#ifndef PUREPURSUITCONTROLLER_HPP
#define PUREPURSUITCONTROLLER_HPP

#include "main.h"
#include "OdomChassis.hpp"
#include "MathUtils/MathUtils.hpp"
#include "okapi/impl/device/distanceSensor.hpp"
#include <memory>

class PurePursuitController{
public:
    PurePursuitController(OdomChassis& o): c{&o} {}
    void run(PurePursuitPath&);
    void run(PurePursuitPath&, DistanceSensor&);
    std::pair<double, double> getVelocities();
    OdomChassis& getChassis(){ return *c; }
    pros::Mutex& getMutex() { return PPMutex; }
    bool hasPath() { return p; }
    PurePursuitPath& getPath() { return *p; }
    DistanceSensor* getDistanceSensor() { return distance; }
    double getLookahead() { return lookahead; }
    void waitUntilSettled() { PPMutex.take(); PPMutex.give(); }
    bool isFollowing() { return following; }
    void setFollowing(bool f) { following = f; }
    Point2D& getGoal() { return goal; }
private:
    Point2D findGoal(double);
    pros::Mutex PPMutex;
    DistanceSensor* distance;
    OdomChassis* c;
    PurePursuitPath* p;
    double lookahead;
    Point2D goal;
    std::unique_ptr<pros::Task> task;
    bool following;
};

#endif