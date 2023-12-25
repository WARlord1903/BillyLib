#include "ControllerBindings/Actions.hpp"
#include "Globals.hpp"

void run_actions(std::map<ControllerDigital, std::vector<MotorGroupAction> >& motor_actions){
    for(auto& pair : motor_actions){
        for(auto& action : pair.second){
            if(action.getToggle()){
                if(controller[pair.first].changedToPressed())
                    action.toggle();
            }
            else{
                if(controller[pair.first].isPressed())
                    action.run();
                else
                    action.stop();
            }
        }
    }
}