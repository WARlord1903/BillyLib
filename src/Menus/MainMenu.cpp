#include "Globals.hpp"
#include "main.h"
#include "Graphics/Menu.hpp"

void mainMenu(Menu& m, void* param){
    m.setFreeze(false);
    std::string option = button_matrix_page(m, {"Controls", "Motor Groups", "Sensors", "\n",
                                                "Chassis", "Auton", "Odom", ""}, false);
    if(option == "Controls")
        m.setCallback(controlMenu1);
    else if(option == "Motor Groups")
        m.setCallback(motorGroupMenu1);
    else if(option == "Sensors")
        ;
    else if(option == "Chassis")
        ;
    else if(option == "Auton")
        m.setCallback(pathPlanner);
    else if(option == "Odom")
        m.setCallback(odomVisualizer);
}