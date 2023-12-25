#include "Graphics/Button.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/Chart.hpp"
#include "Graphics/Slider.hpp"
#include "Graphics/LED.hpp"
#include "Graphics/Label.hpp"
#include "Graphics/Switch.hpp"
#include "Globals.hpp"
#include "OdomChassis.hpp"

struct ModifierData {
    Menu& m;
    double (*modifier)(double);
    double (*old_modifier)(double);
    double scale;
    double old_scale;
};

lv_res_t exit(lv_obj_t *btn){
    ModifierData *data = (ModifierData*) lv_obj_get_free_ptr(btn);
    chassis.setJoystickModifier(data->old_modifier);
    chassis.setJoystickScale(data->old_scale);
    data->m.setCallback(controlMenu1);
    return LV_RES_INV;
}

lv_res_t save(lv_obj_t *btn){
    ModifierData *data = (ModifierData*) lv_obj_get_free_ptr(btn);  
    if(data->modifier == squareModifier)
        ini.set("Drive", "drive_mod", "square");
    else if(data->modifier == cubicModifier)
        ini.set("Drive", "drive_mod", "cubic");
    else if(data->modifier == exponentialModifier)
        ini.set("Drive", "drive_mod", "exponential");
    else
        ini.set("Drive", "drive_mod", "linear");
    ini.set("Drive", "drive_scale", std::to_string(data->scale));
    ini.save();
    data->m.setCallback(controlMenu1);
    return LV_RES_INV;
}

void joystickModifier(Menu& menu, void* param){
    ModifierData data{menu};
    menu.setFreeze(true);
    data.modifier = (double(*)(double)) param;
    data.old_modifier = chassis.getJoystickModifier();
    chassis.setJoystickModifier(data.modifier);
    Chart chart;
    Slider scale_slider;
    LED curr_val(chart);
    Label freeze_switch_label("Enable Robot\n(X to toggle)");
    Switch freeze_switch;
    Button exit_btn;
    Button save_btn;
    curr_val.setSize(6, 6);
    freeze_switch.setSize(60, 20);
    freeze_switch.align(LV_ALIGN_IN_BOTTOM_RIGHT, -30, -10);
    freeze_switch_label.setAlign(LV_LABEL_ALIGN_CENTER);
    freeze_switch_label.align(LV_ALIGN_OUT_TOP_MID, 0, 0, freeze_switch);
    chart.addSeries();
    chart.setType(LV_CHART_TYPE_LINE);
    chart.setRange(0, 24000);
    chart.setSize(LV_VER_RES, LV_VER_RES);
    chart.align(LV_ALIGN_CENTER, 0, 0);
    freeze_switch.setState(false);
    scale_slider.setRange(0, 1000);
    double scale = chassis.getJoystickScale();
    data.old_scale = scale;
    scale_slider.setValue(scale * 1000);
    scale_slider.setSize(20, LV_VER_RES - 40);
    scale_slider.align(LV_ALIGN_IN_LEFT_MID, 0, 0);
    exit_btn.setFreePtr(&data);
    exit_btn.setSize(60, 80);
    exit_btn.align(LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    exit_btn.setAction(exit);
    exit_btn.setText("Exit");
    save_btn.setFreePtr(&data);
    save_btn.setSize(60, 80);
    save_btn.align(LV_ALIGN_OUT_BOTTOM_MID, 0, 0, exit_btn);
    save_btn.setAction(save);
    save_btn.setText("Save");
    std::vector<lv_coord_t> points;
    while(1){
        if(pros::Task::notify_take(true, 0)){
            menu.getTask().notify();
            break;
        }
        if(c_w.toggled(ControllerDigital::X))
            freeze_switch.toggleAnim();
        menu.setFreeze(!freeze_switch.getState());
        if(c_w.toggled(ControllerDigital::Y))
            scale_slider.setValue((int) (spinbox(menu, scale_slider.getValue() / 1000., 4, 3, 0., 1.) * 1000));
        for(double d = -1.; d <= 1.; d += 1. / 127){
            int temp = weightedAvg(linearModifier(d), data.modifier(d), chassis.getJoystickScale())  + 12000;
            lv_coord_t point;
            if(temp > 24000)
                point = 24000;
            else if(temp < 0)
                point = 0;
            else
                point = temp;
            points.push_back(point);
        }
        double joystick = c_w.getAnalog(ControllerAnalog::leftY);
        int index = joystick * 127 + 127;
        chart.setPoints(points, 0);
        curr_val.setPos((chart.getWidth() * ((joystick + 1) / 2)) - 3, (lv_coord_t)(240 - (240. / 24000 * chart.getYVals(0)[index]) - 3));
        chassis.setJoystickScale(scale_slider.getValue() / 1000.);
        data.scale = chassis.getJoystickScale();
        points.clear();
        pros::delay(20);
    }
}