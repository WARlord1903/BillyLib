#include "Graphics/Menu.hpp"
#include "pros/rtos.hpp"

void wait_until_notified(Menu &m){
    while(1){
        if(m.getTask().notify_take(true, 0))
            break;
        pros::delay(20);
    }
}

void runCallback(void* menu_ptr){
    Menu* menu = (Menu*) menu_ptr;
    lv_obj_t* scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);
    while(1){
        if(pros::Task::notify_take(true, 0)){
            if(menu->getCallback())
                menu->getCallback()(*menu, menu->getParam());
        }
        pros::delay(20);
    }
}