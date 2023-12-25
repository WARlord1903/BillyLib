#include "main.h"
#include "MathUtils/PurePursuitPath.hpp"
#include "Globals.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Label.hpp"
#include "Graphics/LED.hpp"
#include "Graphics/Line.hpp"
#include "Graphics/Robot.hpp"
#include "AutonReader.hpp"

struct MessageCmdPair{
    Label* label;
    std::stringstream* cmd;
};

void update_label(void* param){
    MessageCmdPair* pair = (MessageCmdPair*) param;
    while(1){
        if(pros::Task::notify_take(true, 0)){
            pair->label->setText(pair->cmd->str());
            pair->label->realign();
            pair->label->setHidden(false);
            pros::delay(3000);
            pair->label->setHidden(true);
        }
        pros::delay(20);
    }
}

void update_robot(void* robot_ptr){
    Robot* robot = (Robot*) robot_ptr;
    while(1){
        lv_point_t p = odomToPixels(chassis);
        robot->setPos(p.x - robot->get().getWidth() / 2, p.y - robot->get().getHeight() / 2);
        robot->setAngle(chassis.getOrientation());
        if(!robot->isLookaheadHidden()){
            robot->updateLookahead(p_controller.getLookahead());
            pros::delay(80);
        }
        pros::delay(20);
    }
}

void auton_display(std::istream& auton){
    pros::delay(50);
    lv_obj_t* prev_screen = lv_scr_act();
    lv_obj_t* auton_scr = lv_obj_create(NULL, NULL);
    lv_scr_load(auton_scr);
    pros::delay(50);

    AutonReader reader;

    std::string line;
    PurePursuitPath p;

    Image field;
    field.setImagePath("S:/usd/field.bin");
    field.align(LV_ALIGN_CENTER, 0, 0);

    Line path(field);
    path.setHidden(true);

    Robot robot;
    robot.updateLookahead(p_controller.getLookahead());

    pros::Task update_robot_task(update_robot, &robot, "Robot Update Task");

    LED point_led(field);
    point_led.setSize(10, 10);

    Label label;
    label.setHidden(false);
    label.setSize(LV_HOR_RES / 8 - 10, LV_VER_RES / 2);
    label.align(LV_ALIGN_IN_LEFT_MID, 0, 0);

    Label async_label;
    label.setHidden(false);
    label.setSize(LV_HOR_RES / 8 - 10, LV_VER_RES / 2);
    label.align(LV_ALIGN_IN_RIGHT_MID, 0, 0);

    std::stringstream msg;
    std::stringstream async_msg;

    MessageCmdPair pair = {&label, &msg};
    MessageCmdPair async_pair = {&async_label, &async_msg};

    pros::Task update_label_task(update_label, &pair, "Update Message Task");
    pros::Task update_async_label_task(update_label, &async_pair, "Update Async Message Task");
    while(std::getline(auton, line)){
        if(line.find("PATH_BEGIN") != std::string::npos){
            auton >> p;
            reader.run(&p, &back_sensor);
            point_led.setHidden(true);
            path.setPoints(p);
            path.setHidden(false);
            msg << "Following path...";
            update_label_task.notify();
            robot.hideLookahead(false);
            pros::delay(50);
            path.setHidden(true);
            robot.hideLookahead(true);
        }
        else{
            if(line.find("STARTING_POS(") != std::string::npos)
                reader.run(line);
            if(line.find("TURN_TO(") != std::string::npos){
                reader.run(line);
                Point2D point = get_coords(line);
                point_led.setPos(inchesToPixels(point.x), inchesToPixels(point.y));
                point_led.setHidden(false);
                msg << std::fixed << std::setprecision(2) << "Turning to point\n(" << inchesToPixels(point.x) 
                    << ", " << inchesToPixels(point.y) << ")";
                update_label_task.notify();
                chassis.waitUntilSettled();
            }
            else if(line.find("TURN_TO_ASYNC(") != std::string::npos){
                reader.run(line);
                Point2D point = get_coords(line);
                point_led.setPos(inchesToPixels(point.x), inchesToPixels(point.y));
                point_led.setHidden(false);
                async_msg << std::fixed << std::setprecision(2) << "Turning to point\n(" << inchesToPixels(point.x) 
                          << ", " << inchesToPixels(point.y) << ")\nasynchronously";
                update_async_label_task.notify();
            }
            else if(line.find("DRIVE_TO(") != std::string::npos){
                reader.run(line);
                Point2D point = get_coords(line);
                point_led.setPos(inchesToPixels(point.x), inchesToPixels(point.y));
                point_led.setHidden(false);
                msg << std::fixed << std::setprecision(2) << "Driving to point\n(" << inchesToPixels(point.x) 
                    << ", " << inchesToPixels(point.y) << ")";
                update_label_task.notify();
                chassis.waitUntilSettled();
            }
            else if(line.find("DRIVE_TO_ASYNC(") != std::string::npos){
                reader.run(line);
                Point2D point = get_coords(line);
                point_led.setPos(inchesToPixels(point.x), inchesToPixels(point.y));
                point_led.setHidden(false);
                async_msg << std::fixed << std::setprecision(2) << "Driving to point\n(" << inchesToPixels(point.x) 
                          << ", " << inchesToPixels(point.y) << ")\nasynchronously";
                update_async_label_task.notify();
            }
            else if(line.find("CHASSIS_WAIT(") != std::string::npos){
                reader.run(line);
                chassis.waitUntilSettled();
            }
            else if (line.find("PP_WAIT(") != std::string::npos){
                reader.run(line);
                p_controller.waitUntilSettled();
            }
            else if(line.find("TIME_WAIT(") != std::string::npos){
                reader.run(line);
                msg << "Waiting for\n" << get_delay_time(line) << "ms...";
                update_label_task.notify();
                pros::delay(get_delay_time(line));
            }
            else if(line.find("MOVE_DEG(") != std::string::npos){
                reader.run(line);
                auto data = get_motor_group_position(line);
                msg << "Moving " << std::get<0>(data) << " " << std::fixed << std::setprecision(1)
                    << std::get<1>(data) << "\ndegrees at " << std::get<2>(data) << " RPM.";
                update_label_task.notify();
            }
            else if(line.find("MOVE_VOLTAGE(") != std::string::npos){
                reader.run(line);
                auto group_pair = get_motor_group_voltage(line);
                msg << "Moving " << group_pair.first << " at\n" << group_pair.second << " mV.";
                update_label_task.notify();
            }
        }
        pros::delay(20);
        msg.str("");
        async_msg.str("");
    }
    update_robot_task.remove();
    update_label_task.remove();
    update_async_label_task.remove();
    lv_obj_clean(auton_scr);
    lv_scr_load(prev_screen);
    while(pros::competition::is_autonomous())
        pros::delay(20);
}

void auton_display(std::string auton_str){
    std::stringstream auton(auton_str);
    auton_display(auton);
}