#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "main.h"
#include "ControllerBindings/MotorGroupAction.hpp"

void run_actions(std::map<ControllerDigital, std::vector<MotorGroupAction> >&);

#endif