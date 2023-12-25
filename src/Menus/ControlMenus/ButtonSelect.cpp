#include "Graphics/Button.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/Label.hpp"
#include "Globals.hpp"
#include "okapi/impl/device/controllerUtil.hpp"

struct MenuBtn {
    Menu& m;
    ControllerDigital btn;
    bool selected;
};

lv_res_t select_btn_callback(lv_obj_t *btn){
    MenuBtn* data = (MenuBtn*) lv_obj_get_free_ptr(btn);
    data->selected = false;
    data->m.getTask().notify();
    return LV_RES_INV;
}

void btn_check(void* param){
    MenuBtn *data = (MenuBtn*) param;
    while(1){
        for(int i = 6; i < 18; i++){
            if(controller[(ControllerDigital) i].isPressed()){
                data->btn = (ControllerDigital) i;
                data->selected = true;
                data->m.getTask().notify();
            }
        }
        pros::delay(20);
    }
}

void buttonSelect(Menu &m, void* param){
    m.setFreeze(true);
    static MenuBtn data{m};

    Label prompt("Press a button to view\nit's mapping!");
    prompt.align(LV_ALIGN_CENTER, 0, 0);
    pros::Task btn_check_task(btn_check, &data, "Button Check");

    Button select_btn("Manual Selection");
    select_btn.align(LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    select_btn.setFreePtr(&data);
    select_btn.setAction(select_btn_callback);

    wait_until_notified(m);

    btn_check_task.remove();
    if(data.selected)
        m.setCallback(buttonActionMenu, &data.btn);
    else
        m.setCallback(buttonActionMenu);
}