#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "ControllerBindings/Macro.hpp"
#include "ControllerBindings/MotorGroupAction.hpp"
#include "MotorGroup.hpp"
#include "main.h"
#include "OdomChassis.hpp"
#include "Launcher.hpp"
#include "MotionControllers/PurePursuitController.hpp"
#include "MotionControllers/RamseteController.hpp"
#include "ini.hpp"
#include "ControllerWrapper.hpp"
#include "SerialWrapper.hpp"
#include "MotionControllers/MotionProfile.hpp"
#include "Sensors/SensorWrapper.hpp"

extern std::uint32_t control_start;

extern Ini_file ini;

extern Controller controller;

extern ControllerWrapper c_w;

extern Launcher launcher;

extern OdomChassis chassis;

extern PurePursuitController p_controller;
extern RamseteController r_controller;

extern DistanceSensor back_sensor;

extern std::map<int, std::pair<int, bool>> dists;

extern std::map<std::string, Macro> macro_map;

extern std::map<std::string, MotorGroupWrapper> motor_map;
extern std::map<MotorGroupWrapper*, unsigned> motor_count;
extern std::map<ControllerDigital, std::vector<MotorGroupAction> > motor_controller_map;
extern std::map<std::string, MotionProfile> motion_profiles;

extern std::map<std::string, std::unique_ptr<Sensor> > sensor_map;

extern lv_fs_drv_t pcfs_drv;

extern SerialWrapper ser;

#endif