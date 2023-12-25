#include "MotorGroup.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "main.h"
#include "Graphics/ButtonMatrix.hpp"
#include "Graphics/TextArea.hpp"
#include "Graphics/Menu.hpp"
#include "Globals.hpp"
#include <string>

void addMotorGroupMenu(Menu& menu, void* param){
    std::string group_name = keyboard_menu(menu);
    menu.setCallback(editPortsMenu, &group_name);
}