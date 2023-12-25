#include "Globals.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Robot.hpp"
#include "Graphics/Menu.hpp"

void odomVisualizer(Menu& menu, void* param){
    menu.setFreeze(false);
    Image field;
    Robot r;
    field.setImagePath("S:/usd/Field.bin");
    field.align(LV_ALIGN_CENTER, 0, 0);
    while(1){
        if(pros::Task::notify_take(true, 0)){
            menu.getTask().notify();
            break;
        }
        lv_point_t p = odomToPixels(chassis);
        r.setPos(p.x - r.get().getWidth() / 2, p.y - r.get().getHeight() / 2);
        r.setAngle(chassis.getOrientation());
        pros::delay(20);
    }
}