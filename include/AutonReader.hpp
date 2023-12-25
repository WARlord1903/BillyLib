#ifndef AUTONREADER_HPP
#define AUTONREADER_HPP

#include "main.h"
#include "MathUtils/PurePursuitPath.hpp"
#include "okapi/impl/device/distanceSensor.hpp"
#include <string>

void run_auton(void*);

Point2D get_coords(std::string);
std::uint32_t get_delay_time(std::string);
std::pair<std::string, double> get_motor_group_pid(std::string);
std::pair<std::string, std::int16_t> get_motor_group_voltage(std::string);
std::tuple<std::string, double, std::int32_t> get_motor_group_position(std::string);
std::tuple<double, double, double> getPos(std::string);

class AutonReader{
public:
    AutonReader(): auton_task{run_auton, this, "Auton Reader Main"} {}
    void run(std::string& s) { cmd = s; auton_task.notify(); }
    void run(PurePursuitPath* p, DistanceSensor* idistance = nullptr) { cmd = p; distance = idistance; auton_task.notify(); }
    std::variant<std::string, PurePursuitPath*>& getCommand() { return cmd; }
    DistanceSensor* getDistanceSensor() { return distance; }
private:
    std::variant<std::string, PurePursuitPath*> cmd;
    DistanceSensor* distance;
    pros::Task auton_task;
};

#endif