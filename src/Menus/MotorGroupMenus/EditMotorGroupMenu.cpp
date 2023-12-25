#include "Graphics/Label.hpp"
#include "Graphics/Switch.hpp"
#include "MotorGroup.hpp"
#include "display/lv_core/lv_obj.h"
#include "main.h"
#include "Globals.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/Menu.hpp"

struct GroupMenu{
    Menu& m;
    std::string& group;
    bool edit;
};

lv_res_t port_edit_btn(lv_obj_t* btn){
    GroupMenu* data = (GroupMenu*) lv_obj_get_free_ptr(btn);
    data->edit = true;
    data->m.getTask().notify();
    return LV_RES_INV;
}

lv_res_t exit_btn_callback(lv_obj_t* btn){
    GroupMenu* data = (GroupMenu*) lv_obj_get_free_ptr(btn);
    data->edit = false;
    data->m.getTask().notify();
    return LV_RES_INV;
}

void editMotorGroupMenu(Menu& menu, void* param){
    static std::string group_name;
    std::string* group_name_ptr = (std::string*) param;
    if(group_name_ptr)
        group_name = *group_name_ptr;
    else{
        std::vector<std::string> group_names;
        for(auto& pair : motor_map){
            group_names.push_back(pair.first);
            group_names.push_back("\n");
        }
        group_name = button_matrix_page(menu, group_names, false);
    }

    MotorGroupWrapper curr = motor_map[group_name];
    
    GroupMenu data = {menu, group_name};

    Label group_label(group_name);
    group_label.align(LV_ALIGN_IN_TOP_MID, 0, 0);

    std::string port_str = "Ports: ";
    for(auto& p : curr.getPorts())
        port_str += std::to_string(p) + ", ";
    if(port_str != "Ports: ")
        port_str = port_str.substr(0, port_str.length() - 2);

    Label ports_label(port_str);
    ports_label.align(LV_ALIGN_CENTER, 0, 0);

    Button edit_ports("Edit Ports");
    edit_ports.align(LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    edit_ports.setFreePtr(&data);
    edit_ports.setAction(port_edit_btn);

    Button exit_btn("Exit");
    exit_btn.align(LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    exit_btn.setFreePtr(&data);
    exit_btn.setAction(exit_btn_callback);

    wait_until_notified(menu);

    if(data.edit)
        menu.setCallback(editPortsMenu, (void*) &group_name);
    else
        menu.setCallback(mainMenu);
}