#include "Globals.hpp"
#include "Graphics/Menu.hpp"

void controlMenu1(Menu &m, void *param){
    std::string option = button_matrix_page(m, {"Joystick\nModifier", "Macro\nEditor", "Button\nMapping", ""});
    if(option == "Joystick\nModifier")
        m.setCallback(joystickModifierSelect);
    else if(option == "Macro\nEditor")
        m.setCallback(macroEditor);
    else if(option == "Button\nMapping")
        m.setCallback(buttonSelect);
    else
        m.setCallback(mainMenu);
}