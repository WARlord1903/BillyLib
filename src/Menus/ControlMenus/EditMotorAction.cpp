#include "ControllerBindings/MotorGroupAction.hpp"
#include "Globals.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/TextArea.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_ta.h"
#include <string>

struct MenuAction {
    Menu& m;
    MotorGroupAction* action;
    bool change_group;
    bool change_voltage;
};

lv_res_t update_group(lv_obj_t *obj){
    MenuAction* data = (MenuAction*) lv_obj_get_free_ptr(obj);
    data->change_group = true;
    data->m.getTask().notify();
    return LV_RES_INV;
}

lv_res_t update_voltage(lv_obj_t* obj){
    MenuAction* data = (MenuAction*) lv_obj_get_free_ptr(obj);
    data->change_voltage = true;
    data->m.getTask().notify();
    return LV_RES_INV;
}


lv_res_t save_btn_callback(lv_obj_t* obj){
    Menu* m = (Menu*) lv_obj_get_free_ptr(obj);
    m->getTask().notify();
    return LV_RES_INV;
}

void editMotorAction(Menu &m, void* action_ptr){
    static MotorGroupAction* action = (MotorGroupAction*) action_ptr;
    MenuAction data = {m, action};

    TextArea motor_group_display;
    motor_group_display.setText(action->getName());
    motor_group_display.setSize(160, 50);
    motor_group_display.setCursorType(LV_CURSOR_NONE);
    motor_group_display.align(LV_ALIGN_CENTER, 0, 0);
    motor_group_display.setFreePtr(&data);
    motor_group_display.setAction(update_group);

    TextArea voltage_display;
    voltage_display.setText(std::to_string(action->getVoltage()) + " mV");
    voltage_display.setSize(160, 50);
    voltage_display.setCursorType(LV_CURSOR_NONE);
    voltage_display.align(LV_ALIGN_CENTER, 0, 100);
    voltage_display.setFreePtr(&data);
    voltage_display.setAction(update_voltage);

    Button save_btn("Save");
    save_btn.align(LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    save_btn.setFreePtr(&m);
    save_btn.setAction(save_btn_callback);


    wait_until_notified(m);
    
    if(data.change_group){
        std::vector<std::string> groups;
        for(auto& pair : motor_map){
            groups.push_back(pair.first);
            groups.push_back("\n");
        }
        groups.push_back("");

        std::string group = button_matrix_page(m, groups, false);
        action->setName(group);
        m.setCallback(editMotorAction, action);
    }
    else if(data.change_voltage){
        action->setVoltage(spinbox(m, action->getVoltage(), 5, -12000, 12000, "Enter new voltage (mV)"));
        m.setCallback(editMotorAction, action);
    }
    else{
        write_motor_actions(ini);
        m.setCallback(mainMenu);
    }
}