#include "display/lv_conf.h"
#include "main.h"
#include "Graphics/Menu.hpp"

void motorGroupMenu1(Menu& menu, void* param){
    std::string selection = button_matrix_page(menu, {"Edit Motor\nGroup", "Add Motor\nGroup", "Delete Motor\nGroup", ""});

    if(selection == "Edit Motor\nGroup")
        menu.setCallback(editMotorGroupMenu);
    else if(selection == "Add Motor\nGroup")
        menu.setCallback(addMotorGroupMenu);
}