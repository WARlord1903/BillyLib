#include "Globals.hpp"
#include "Graphics/Menu.hpp"

void joystickModifierSelect(Menu &m, void* param){
    std::string option = button_matrix_page(m, {"Linear", "Exponential", "\n",
                                                "Squared", "Cubic", "\n",
                                                "Current Modifier", ""}, false);
    if(option == "Linear")
        m.setCallback(joystickModifier, (void*) linearModifier);
    else if(option == "Exponential")
        m.setCallback(joystickModifier, (void*) exponentialModifier);
    else if(option == "Squared")
        m.setCallback(joystickModifier, (void*) squareModifier);
    else if(option == "Cubic")
        m.setCallback(joystickModifier, (void*) cubicModifier);
    else
        m.setCallback(joystickModifier, (void*) chassis.getJoystickModifier());
}