#include "Globals.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/ButtonMatrix.hpp"
#include "Graphics/Pointer.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/MessageBox.hpp"
#include "Graphics/Keyboard.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Label.hpp"
#include "Graphics/Robot.hpp"
#include "Graphics/Slider.hpp"
#include "Graphics/LED.hpp"
#include "Graphics/FieldGrid.hpp"
#include "display/lv_conf.h"
#include "display/lv_core/lv_obj.h"
#include "okapi/impl/device/controllerUtil.hpp"
#include <iterator>
#include <string>
#include <vector>

lv_res_t align_robot(lv_obj_t* btn){
    Robot* robot = (Robot*) lv_obj_get_free_ptr(btn);
    robot->align(LV_ALIGN_CENTER, 0, 0, btn);
    return LV_RES_OK;
}

lv_res_t exit_menu(lv_obj_t* obj){
    Menu* m = (Menu*) lv_obj_get_free_ptr(obj);
    m->getTask().notify();
    return LV_RES_OK;
}

struct AngleData{
    double* d;
    Menu* m;
};

lv_res_t apply_angle(lv_obj_t* btn){
    AngleData* data = (AngleData*) lv_obj_get_free_ptr(btn);
    switch(lv_obj_get_free_num(btn)){
        case 0:
            *(data->d) = 0;
            break;
        case 1:
            *(data->d) = M_PI / 2.;
            break;
        case 2:
            *(data->d) = M_PI;
            break;
        case 3:
            *(data->d) = 3 * M_PI / 2.;
            break;
    }
    data->m->getTask().notify();
    return LV_RES_INV;
}

Point2D select_point(Menu& menu, Robot& r, std::vector<Line>& auton_lines){
    lv_obj_t* prev_scr = lv_scr_act();
    lv_obj_t* point_scr = lv_obj_create(NULL, NULL);
    lv_scr_load(point_scr);

    Image field;
    field.setImagePath("S:/usd/Field.bin");
    field.align(LV_ALIGN_CENTER, 0, 0);

    Robot robot;
    robot.setPos(r.get().getX(), r.get().getY());

    Pointer pointer;

    int x = LV_HOR_RES / 2 - LV_HOR_RES / 4;
    int y = LV_VER_RES / 2;

    while(1){
        if(c_w.toggled(ControllerDigital::A)){
            lv_scr_load(prev_scr);
            lv_obj_del(point_scr);
            return pixelsToOdom(x, y);
        }
        while(c_w.isPressed(ControllerDigital::down) && y >=  4){
            y -= 4;
            field.setY(field.getY() - 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::up) && y <= LV_VER_RES - 4){
            y += 4;
            field.setY(field.getY() + 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::right) && x <= 236){
            x += 4;
            field.setX(field.getX() - 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::left) && x >= 4){
            x -= 4;
            field.setX(field.getX() + 4);
            pros::delay(230);
        }
        pros::delay(20);
    }
}

struct StrMenu{
    Menu* m;
    std::string* s;
};

std::array<double, 4> getRerouteBounds(Menu& menu){
    lv_obj_t* prev_screen = lv_scr_act();
    lv_obj_t* bounds_screen = lv_obj_create(NULL, NULL);
    lv_scr_load(bounds_screen);

    Image field;
    field.setImagePath("S:/usd/Field.bin");
    field.align(LV_ALIGN_CENTER, 0, 0);

    std::array<Line, 4> lines;
    std::array<double, 4> bounds;

    Label text;
    text.align(LV_ALIGN_IN_TOP_MID, 0, 0);

    for(int i = 0; i < lines.size(); i++){
        switch(i){
            case 0:
                text.setText("Enter minimum x-axis boundary.");
                lines[i].setPoints({{field.getX(), field.getY()}, {field.getX(), (lv_coord_t) (field.getY() + field.getHeight())}});
                break;
            case 1:
                text.setText("Enter maximum x-axis boundary.");
                lines[i].setPoints({{(lv_coord_t) (lines[0].getPoints()[0].x + 4), lines[0].getPoints()[0].y}, {(lv_coord_t) (lines[0].getPoints()[1].x + 4), lines[0].getPoints()[1].y}});
                break;
            case 2:
                text.setText("Enter minimum y-axis boundary.");
                lines[i].setPoints({{field.getX(), field.getY()}, {(lv_coord_t) (field.getX() + field.getWidth()), field.getY()}});
                break;
            case 3:
                text.setText("Enter maximum y-axis boundary.");
                lines[i].setPoints({{lines[2].getPoints()[0].x, (lv_coord_t) (lines[2].getPoints()[0].y + 4)}, {lines[2].getPoints()[1].x, (lv_coord_t) (lines[2].getPoints()[1].y + 4)}});
                break;
        }

        while(1){
            if(pros::Task::notify_take(true, 0))
                break;
            if(c_w.toggled(ControllerDigital::A))
                menu.getTask().notify();
            while(c_w.isPressed(ControllerDigital::left) && i <= 1 && lines[i].getPoints()[0].x > ((i == 0) ? field.getX() : lines[0].getPoints()[0].x)){
                lv_point_t pt1 = lines[i].getPoints()[0];
                lv_point_t pt2 = lines[i].getPoints()[1];
                lines[i].setPoints({{(lv_coord_t) (pt1.x - 4), pt1.y}, {(lv_coord_t) (pt2.x - 4), pt2.y}});
                pros::delay(250);
            }
            while(c_w.isPressed(ControllerDigital::right) && i <= 1 && lines[i].getPoints()[0].x < ((i == 0) ? field.getX() + field.getWidth() - 4 : field.getX() + field.getWidth())){
                lv_point_t pt1 = lines[i].getPoints()[0];
                lv_point_t pt2 = lines[i].getPoints()[1];
                lines[i].setPoints({{(lv_coord_t) (pt1.x + 4), pt1.y}, {(lv_coord_t) (pt2.x + 4), pt2.y}});
                pros::delay(250);
            }
            while(c_w.isPressed(ControllerDigital::up) && i >= 2 && lines[i].getPoints()[0].y > ((i == 2) ? field.getY() + 4 : field.getY())){
                lv_point_t pt1 = lines[i].getPoints()[0];
                lv_point_t pt2 = lines[i].getPoints()[1];
                lines[i].setPoints({{pt1.x, (lv_coord_t) (pt1.y - 4)}, {pt2.x, (lv_coord_t) (pt2.y - 4)}});
                pros::delay(250);
            }
            while(c_w.isPressed(ControllerDigital::down) && i >= 2 && lines[i].getPoints()[0].y < ((i == 2) ? LV_VER_RES - field.getY() : lines[2].getPoints()[0].y)){
                lv_point_t pt1 = lines[i].getPoints()[0];
                lv_point_t pt2 = lines[i].getPoints()[1];
                lines[i].setPoints({{pt1.x, (lv_coord_t) (pt1.y + 4)}, {pt2.x, (lv_coord_t) (pt2.y + 4)}});
                pros::delay(250);
            }
            pros::delay(20);
        }
        if(i <= 1)
            bounds[i] = pixelsToOdom(lines[i].getPoints()[0].x, lines[i].getPoints()[0].y).x;
        else
            bounds[i] = pixelsToOdom(lines[i].getPoints()[0].x, lines[i].getPoints()[0].y).y;
    }
    lv_scr_load(prev_screen);
    lv_obj_del(bounds_screen);
    return bounds;
}

lv_res_t select_auton(lv_obj_t* btn){
    Menu* m = (Menu*) lv_obj_get_free_ptr(btn);
    std::vector<std::string> names;
    for(auto& s : ini.getKeys("Auton Paths")){
        names.push_back(s);
        names.push_back("\n");
    }
    if(names.empty())
        return LV_RES_OK;
    std::string sel = button_matrix_page(*m, names, false);
    ini.set("Config", "curr_auton", ini.get("Auton Paths", sel, ""));
    m->setCallback(mainMenu);
    return LV_RES_INV;
}

lv_res_t load_curr_auton(lv_obj_t* mbox, const char* txt){
    StrMenu* data = (StrMenu*) lv_obj_get_free_ptr(mbox);
    if(strcmp("Yes", txt) == 0){
        ini.set("Config", "curr_auton", *(data->s));
        data->m->getTask().notify();
    }
    return LV_RES_INV;
}

void pathPlanner(Menu& menu, void* param){
    menu.setFreeze(true);

    std::stringstream auton;

    Image field;
    field.setHidden(true);

    FieldGrid field_grid;

    Robot robot;
    robot.hideHeading(true);

    field_grid.setFreePtr(&robot);
    field_grid.setAction(align_robot);

    Button exit("Save");
    exit.setSize(120, 80);
    exit.align(LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    exit.setFreePtr(&menu);
    exit.setAction(exit_menu);

    Button load("Load");
    load.setSize(120, 80);
    load.align(LV_ALIGN_IN_RIGHT_MID, 0, 0);
    load.setFreePtr(&menu);
    load.setAction(select_auton);

    wait_until_notified(menu);

    pros::delay(200);
    field_grid.setAction(nullptr);
    double angle;
    AngleData data = {&angle, &menu};
    std::array<Button, 4> arrow_btns;
    for(int i = 0; i < arrow_btns.size(); i++){
        arrow_btns[i].setFreeNum(i);
        arrow_btns[i].setFreePtr(&data);
        arrow_btns[i].setAction(apply_angle);
        arrow_btns[i].setSize(30, 30);
    }
    arrow_btns[0].align(LV_ALIGN_OUT_RIGHT_MID, 3, 0, robot.get());
    arrow_btns[0].setText(SYMBOL_RIGHT);
    arrow_btns[1].align(LV_ALIGN_OUT_TOP_MID, 0, -3, robot.get());
    arrow_btns[1].setText(SYMBOL_UP);
    arrow_btns[2].align(LV_ALIGN_OUT_LEFT_MID, -3, 0, robot.get());
    arrow_btns[2].setText(SYMBOL_LEFT);
    arrow_btns[3].align(LV_ALIGN_OUT_BOTTOM_MID, 0, 3, robot.get());
    arrow_btns[3].setText(SYMBOL_DOWN);

    wait_until_notified(menu);

    pros::delay(200);

    field_grid.del();
    for(auto& b : arrow_btns)
        b.del();
    exit.del();

    Point2D start_point = pixelsToOdom((robot.get().getX() + robot.get().getWidth() / 2) - (LV_HOR_RES / 4), LV_VER_RES - (robot.get().getY() + robot.get().getHeight() / 2));
    auton << "STARTING_POS(" << start_point.x << ", " << start_point.y << ", " << angle << ")\n";

    int x = LV_HOR_RES / 2 - LV_HOR_RES / 4;
    int y = LV_VER_RES / 2;
    std::vector<Line> auton_line;
    Pointer pointer;
    Slider weight_slider;
    Slider space_slider;
    Label weight_label_top;
    Label weight_label_bottom;
    Label space_label_top;
    Label space_label_bottom;
    std::vector<LED> points;
    PurePursuitPath p{{start_point}};

    bool joystick_set = true;

    auton_line.emplace_back(field);

    weight_slider.setHidden(true); 
    space_slider.setHidden(true); 

    weight_slider.setSize(10, LV_VER_RES - 40);
    space_slider.setSize(10, LV_VER_RES - 40);

    weight_slider.align(LV_ALIGN_IN_RIGHT_MID, -60, 0);
    space_slider.align(LV_ALIGN_IN_LEFT_MID, 55, 0);

    weight_slider.setRange(0, 98);
    space_slider.setRange(6, 24);

    weight_label_top.setHidden(true);
    weight_label_bottom.setHidden(true);
    space_label_top.setHidden(true);
    space_label_bottom.setHidden(true);

    weight_label_top.setText("Smoothing");
    space_label_top.setText("Spacing");
    weight_label_bottom.setText(std::to_string(weight_slider.getValue() / 100.).substr(0, 4));
    space_label_bottom.setText(std::to_string(space_slider.getValue() / 2.).substr(0, 3) + "\"");

    weight_label_top.align(LV_ALIGN_CENTER, 0, -(LV_VER_RES / 2 - 10), weight_slider);
    weight_label_bottom.align(LV_ALIGN_CENTER, 0, LV_VER_RES / 2 - 20, weight_slider);
    space_label_top.align(LV_ALIGN_CENTER, 0, -(LV_VER_RES / 2 - 10), space_slider);
    space_label_bottom.align(LV_ALIGN_CENTER, 0, LV_VER_RES / 2 - 20, space_slider);

    field.setImagePath("S:/usd/Field.bin");
    field.setHidden(false);
    field.align(LV_ALIGN_CENTER, 0, 0); 

    robot.get().setParent(field);
    robot.get().setX(robot.get().getX() - LV_HOR_RES / 4);

    weight_slider.setValue(75);
    space_slider.setValue(6);

    while(1){
        if(pros::Task::notify_take(true, 0)){
            menu.getTask().notify();
            break;
        }
        while(c_w.isPressed(ControllerDigital::down) && y >=  4){
            y -= 4;
            field.setY(field.getY() - 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::up) && y <= LV_VER_RES - 4){
            y += 4;
            field.setY(field.getY() + 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::right) && x <= 236){
            x += 4;
            field.setX(field.getX() - 4);
            pros::delay(230);
        }
        while(c_w.isPressed(ControllerDigital::left) && x >= 4){
            x -= 4;
            field.setX(field.getX() + 4);
            pros::delay(230);
        }
        if(c_w.toggled(ControllerDigital::A)){
            points.emplace_back(field);
            points.back().setSize(5, 5);
            points.back().align(LV_ALIGN_CENTER, 0, 0);
            p.push_back_orig(pixelsToOdom(x, y));
        }
        if(c_w.toggled(ControllerDigital::B)){
            if(!points.empty()){
                points.back().del();
                points.pop_back();
                p.pop_back_orig();
            }
        }
        if(c_w.toggled(ControllerDigital::L1)){
            std::string command;
            std::string action = button_matrix_page(menu, {"Wait For\nTime", "Wait For\nChassis", "\n", 
                                                            "Wait For\nPure Pursuit", "Move Motor\nVoltage", "\n",
                                                            "Move Motor\nDegrees", "Move Motor\nPID", "\n",
                                                            "Turn To\nPoint", "Turn To\nPoint Async",  "\n", 
                                                            "Drive To\nPoint", "Drive To\nPoint Async", ""});
            if(action == "Wait For\nTime"){
                int time = spinbox(menu, 0, 5, 0, 45000, "Enter Delay Time");
                command = "TIME_WAIT(" + std::to_string(time) + ")";
            }
            else if(action == "Wait For\nChassis")
                command = "CHASSIS_WAIT()\n";
            else if(action == "Wait For\nPure Pursuit")
                command = "PP_WAIT()\n";
            else if(action == "Turn To\nPoint"){
                command = "TURN_TO(";
                Point2D point = select_point(menu, robot, auton_line);
                command += std::to_string(point.x) + ", " + std::to_string(point.y)+ ")";
            }
            else if(action == "Turn To\nPoint Async"){
                command = "TURN_TO_ASYNC(";
                Point2D point = select_point(menu, robot, auton_line);
                command += std::to_string(point.x) + ", " + std::to_string(point.y)+ ")";
            }
            else if(action == "Drive To\nPoint"){
                command = "DRIVE_TO(";
                Point2D point = select_point(menu, robot, auton_line);
                command += std::to_string(point.x) + ", " + std::to_string(point.y)+ ")";
                auton_line.back().setPoints({p.back_orig(), point});
                p.clear();
                p.push_back_orig(point);
                auton_line.emplace_back(field);
            }
            else if(action == "Drive To\nPoint Async"){
                command = "DRIVE_TO_ASYNC(";
                Point2D point = select_point(menu, robot, auton_line);
                command += std::to_string(point.x) + ", " + std::to_string(point.y)+ ")";
                auton_line.back().setPoints({p.back_orig(), point});
                p.clear();
                p.push_back_orig(point);
                auton_line.emplace_back(field);
            }
            else if(action == "Move Motor\nDegrees"){
                std::vector<std::string> motor_group_keys;
                for(auto& p : motor_map){
                    motor_group_keys.push_back(p.first);
                    motor_group_keys.push_back("\n");
                }
                std::string group = button_matrix_page(menu, motor_group_keys);
                if(group != "Exit"){
                    double degrees = spinbox(menu, 0, 6, 2, -INFINITY, INFINITY, "Enter the number of degrees to move.");
                    std::int32_t velocity = spinbox(menu, 200, 3, 0, 600, "Enter desired velocity");
                    command = "MOVE_DEG(" + group + ", " + std::to_string(degrees) + ", " + std::to_string(velocity) + ")";
                }
            }
            else if(action == "Move Motor\nVoltage"){
                std::vector<std::string> motor_group_keys;
                for(auto& p : motor_map){
                    motor_group_keys.push_back(p.first);
                    motor_group_keys.push_back("\n");
                }
                std::string group = button_matrix_page(menu, motor_group_keys);
                if(group != "Exit"){
                    std::int16_t voltage = spinbox(menu, 0, 5, -12000, 12000, "Enter desired voltage");
                    command = "MOVE_VOLTAGE(" + group + ", " + std::to_string(voltage) + ")";
                }
            }
            auton << command << "\n";
        }
        if(c_w.toggled(ControllerDigital::R1) && points.size() > 1){
            int sel = 0;
            field.align(LV_ALIGN_CENTER, 0, 0);
            weight_slider.setHidden(false);
            space_slider.setHidden(false);
            weight_label_top.setHidden(false);
            space_label_top.setHidden(false);
            weight_label_bottom.setHidden(false);
            space_label_bottom.setHidden(false);
            for(auto& l : points)
                l.setHidden(true);
            space_label_bottom.setText(std::to_string(space_slider.getValue() / 2.).substr(0, 3) + "\"");
            space_label_bottom.realign();
            weight_label_bottom.setText(std::to_string(weight_slider.getValue() / 100.).substr(0, 4));
            weight_label_bottom.realign();
            p.injectPoints(space_slider.getValue() / 2.);
            p.smoothPath(weight_slider.getValue() / 100.);
            while(1){
                space_label_bottom.setText(std::to_string(space_slider.getValue() / 2.).substr(0, 3) + "\"");
                space_label_bottom.realign();
                weight_label_bottom.setText(std::to_string(weight_slider.getValue() / 100.).substr(0, 4));
                weight_label_bottom.realign();
                if(c_w.toggled(ControllerDigital::L1) && c_w.isPressed(ControllerDigital::R1))
                    joystick_set = !joystick_set;
                if(joystick_set){
                    space_slider.setValue((1 + c_w.getAnalog(ControllerAnalog::leftY)) * ((space_slider.getMax() - space_slider.getMin()) + space_slider.getMin()) / 2.0);
                    weight_slider.setValue((1 + c_w.getAnalog(ControllerAnalog::rightY)) * ((weight_slider.getMax() - weight_slider.getMin()) + weight_slider.getMin()) / 2.0);
                }
                p.injectPoints(space_slider.getValue() / 2.);
                p.smoothPath(weight_slider.getValue() / 100.);
                auton_line.back().setPoints(p);
                if(c_w.toggled(ControllerDigital::B)){
                    weight_slider.setHidden(true);
                    space_slider.setHidden(true);
                    weight_label_top.setHidden(true);
                    weight_label_bottom.setHidden(true);
                    space_label_top.setHidden(true);
                    space_label_bottom.setHidden(true);
                    auton_line.back().setPoints({});
                    x = LV_HOR_RES / 2 - 120;
                    y = LV_VER_RES / 2;
                    for(auto& l : points)
                        l.setHidden(false);
                    joystick_set = true;
                    break;
                }
                if(c_w.toggled(ControllerDigital::A)){
                    pros::delay(200);
                    double minLookahead = spinbox(menu, 3.0, 3, 1, 3.0, 18.0, "Enter the minimum Lookahead.");
                    double maxLookahead = spinbox(menu, minLookahead, 3, 1, minLookahead, 24.0, "Enter the maximum Lookahead.");
                    bool reverse = spinbox(menu, 0, 1, 0, 1, "Is this path reversed?");
                    // double reroute = spinbox(menu, 0., 3, 2, 0., 24., "Enter reroute distance (0 for no avoidance).");
                    p.setMinLookahead(minLookahead);
                    p.setMaxLookahead(maxLookahead);
                    p.setReversed(reverse);
                    // p.setReroute(reroute);
                    auton << p;
                    Point2D back = p.back();
                    p.clear();
                    p.push_back_orig(back);
                    weight_slider.setValue(75);
                    space_slider.setValue(6);
                    weight_slider.setHidden(true);
                    space_slider.setHidden(true);
                    weight_label_top.setHidden(true);
                    weight_label_bottom.setHidden(true);
                    space_label_top.setHidden(true);
                    space_label_bottom.setHidden(true);
                    auton_line.emplace_back(field);
                    robot.setPos(inchesToPixels(back.x) - (int) (robot.get().getWidth() / 2), LV_VER_RES - inchesToPixels(back.y) - (int) (robot.get().getHeight() / 2));
                    x = LV_HOR_RES / 2 - 120;
                    y = LV_VER_RES / 2;
                    for(auto& l : points)
                        l.del();
                    points.clear();
                    joystick_set = true;
                    break;
                }
                pros::delay(20);
            }
        }
        if(c_w.toggled(ControllerDigital::R2)){
            std::string auton_name = keyboard_menu(menu);

            std::string auton_path = "/usd/" + auton_name + ".aut";

            std::ofstream ofs(auton_path);
            std::copy(
                       std::istreambuf_iterator<char>(auton), 
                       std::istreambuf_iterator<char>(), 
                       std::ostreambuf_iterator<char>(ofs)
            );
            ofs.close();

            ini.set("Auton Paths", auton_name, auton_path);
            ini.save();

            // StrMenu data = {&menu, &auton_path};

            // MessageBox load_curr("Would you like to load\nthis auton routine?", {"Yes", "No"});
            // load_curr.setSize(LV_HOR_RES, LV_VER_RES);
            // load_curr.setFreePtr(&data);
            // load_curr.align(LV_ALIGN_CENTER, 0, 0);
            // load_curr.setAction(load_curr_auton);

            // wait_until_notified(menu);

            menu.setCallback(mainMenu);
        }
        pros::delay(20);
    }
}