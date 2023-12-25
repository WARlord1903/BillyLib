#include "Globals.hpp"
#include "ControllerBindings/MotorGroupAction.hpp"
#include "MotionControllers/MotionProfile.hpp"
#include "MotionControllers/RamseteController.hpp"
#include "MotorGroup.hpp"
#include "Sensors/SensorWrapper.hpp"
#include "okapi/impl/device/distanceSensor.hpp"
#include <vector>

std::uint32_t control_start;

Ini_file ini("/usd/config.ini");

Controller controller;

ControllerWrapper c_w;

Launcher launcher({1, -2}, pros::ADIDigitalOut(3), pros::ADIDigitalOut(2));

OdomChassis chassis;

PurePursuitController p_controller(chassis);

RamseteController r_controller(chassis);

DistanceSensor back_sensor(5);

std::map<std::string, Macro> macro_map;

std::map<std::string, MotorGroupWrapper> motor_map;
std::map<MotorGroupWrapper*, unsigned> motor_count;
std::map<okapi::ControllerDigital, std::vector<MotorGroupAction> > motor_controller_map;
std::map<std::string, MotionProfile> motion_profiles;

std::map<std::string, std::unique_ptr<Sensor> > sensor_map;

std::map<int, std::pair<int, bool> > dists(
    {
        {3, {8000, true}},
        {4, {8500, true}},
        {5, {8800, true}},
        {6, {9500, true}},
        {7, {10000, true}},
        {8, {10500, true}},
        {9, {11000, true}},
        {10, {11500, true}},
        {11, {12000, true}},
    }
);

lv_fs_drv_t pcfs_drv;

SerialWrapper ser(14);