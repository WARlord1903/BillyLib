#include "main.h"
#include "ControllerBindings/Actions.hpp"
#include "ControllerBindings/Macro.hpp"
#include "ControllerBindings/MotorGroupAction.hpp"
#include "MathUtils/Matrix.hpp"
#include "MathUtils/Point2D.hpp"
#include "MathUtils/RamsetePath.hpp"
#include "MotionControllers/DistancePIDController.hpp"
#include "Globals.hpp"
#include "OdomChassis.hpp"
#include "MathUtils/PurePursuitPath.hpp"
#include "MotionControllers/MotionProfile.hpp"
#include "MotionControllers/PurePursuitController.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "types.hpp"
#include "Graphics/Menu.hpp"
#include "MotorGroup.hpp"
#include <fstream>

/* IMG DRIVER SETUP */

typedef FILE* pc_file_t;

static lv_fs_res_t pcfs_open( void * file_p, const char * fn, lv_fs_mode_t mode)
{
    errno = 0;
    const char * flags = "";
    if(mode == LV_FS_MODE_WR) flags = "wb";
    else if(mode == LV_FS_MODE_RD) flags = "rb";
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "a+";

    char buf[256];
    sprintf(buf, "/%s", fn);
    pc_file_t f = fopen(buf, flags);

    if(f == NULL)
      return LV_FS_RES_UNKNOWN;
    else {
      fseek(f, 0, SEEK_SET);
      pc_file_t * fp = (pc_file_t *)file_p;
      *fp = f;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_close( void * file_p)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fclose(*fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_read( void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *br = fread(buf, 1, btr, *fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_seek( void * file_p, uint32_t pos)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fseek(*fp, pos, SEEK_SET);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_tell( void * file_p, uint32_t * pos_p)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *pos_p = ftell(*fp);
    return LV_FS_RES_OK;
}

/********************/

void printOdom(void *ign){
    std::ofstream ofs("/usd/odom.csv");
    ofs << "Time,X,Y,Orientation\n";
    while(pros::millis() < 7500){
        ofs << pros::millis() << "," << chassis.getX() << "," << chassis.getY() << "," << chassis.getOrientation() * (180 / M_PI) << "\n";
        pros::delay(5);
    }
    ofs.close();
}

void printAngle(void *ign){
    std::ofstream ofs("/usd/odom.csv");
    ofs << "Time,Orientation\n";
    while(pros::millis() < 5000){
        ofs << pros::millis() << "," << chassis.getOrientation() * (180 / M_PI) << "\n";
        pros::delay(5);
    }
    ofs.close();
}

void printOdomController(void *ign){
    controller.clear();
    while(1){
        std::string xtext = "X: " + std::to_string(chassis.getX());
        std::string ytext = "Y: " + std::to_string(chassis.getY());
        std::string angletext = "Angle: " + std::to_string(chassis.getOrientation() * (180 / M_PI));
        controller.setText(0, 0, xtext);
        pros::delay(51);
        controller.setText(1, 0, ytext);
        pros::delay(51);
        controller.setText(2, 0, angletext);
        pros::delay(51);
    }
}

void printDistanceSensor(void* sensor_ptr){
    DistanceSensor* sensor = (DistanceSensor*) sensor_ptr;
    controller.clear();
    while(1){
        std::string text = "Dist: " + std::to_string(sensor->get() / 25.4);
        controller.setText(0, 0, text);
        pros::delay(51);
    }
}

void printTargetController(void *ign){
    controller.clear();
    while(1){
        std::string xtext = "Dist: " + std::to_string(std::sqrt(std::pow(17.78 - chassis.getX(), 2) + std::pow(17.78 - chassis.getY(), 2)) / 12.0);
        std::string ytext = "Voltage: " + std::to_string(launcher.getFlywheel().getMotors().getVoltage());
        std::string angletext = "Angle: " + std::to_string(chassis.getOrientation() * (180 / M_PI));
        controller.setText(0, 0, xtext);
        pros::delay(51);
        controller.setText(1, 0, ytext);
        pros::delay(51);
        controller.setText(2, 0, angletext);
        pros::delay(51);
    }
}

void printEncodersController(void *ign){
    controller.clear();
    while(1){
        std::string xtext = "Left: " + std::to_string(chassis.getLeftEncoder().get());
        std::string ytext = "Middle: " + std::to_string(chassis.getMidEncoder().get());
        std::string angletext;
        if(std::holds_alternative<IMUPtr>(chassis.getAngular()))
            angletext = "IMU: " + std::to_string(std::get<IMUPtr>(chassis.getAngular())->get_heading());
        else
            angletext = "Right: " + std::to_string(std::get<ADIEncoderPtr>(chassis.getAngular())->get());
        controller.setText(0, 0, xtext);
        pros::delay(51);
        controller.setText(1, 0, ytext);
        pros::delay(51);
        controller.setText(2, 0, angletext);
        pros::delay(51);
    }
}

void save_auton(void* ofs_ptr){
    PurePursuitPath last_path;
    std::ofstream ofs("/usd/last_auton.txt");
    std::stringstream state;
    int curr_path_index = -1;
    int last_path_index = -1;
    ser.write("AUTONBEGIN\n");
    while(true){
        if(p_controller.hasPath() && p_controller.getPath() != last_path){
            curr_path_index = last_path_index + 1;
            last_path = p_controller.getPath();
            state << last_path;
        }
        if(!p_controller.isFollowing()){
            last_path_index = curr_path_index;
            curr_path_index = -1;
        }
        state << pros::millis() - control_start << ", " << chassis.getX() << ", " << chassis.getY() << ", " << chassis.getOrientation() << ", " << curr_path_index << ", ";
        if(p_controller.isFollowing())
            state << p_controller.getLookahead() << ", " << p_controller.getGoal() << "\n";
        else
            state << "-1, -999, -999\n";
        ofs << state.str();
        ser.write(state.str());
        state.str("");
        pros::delay(75);
        if(pros::Task::notify_take(true, 0))
            break;
    }
    ser.write("END\n");
    ofs.close();
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    memset(&pcfs_drv, 0, sizeof(lv_fs_drv_t));    /*Initialization*/

    pcfs_drv.file_size = sizeof(pc_file_t);       /*Set up fields...*/
    pcfs_drv.letter = 'S';
    pcfs_drv.open = pcfs_open;
    pcfs_drv.close = pcfs_close;
    pcfs_drv.read = pcfs_read;
    pcfs_drv.seek = pcfs_seek;
    pcfs_drv.tell = pcfs_tell;
    lv_fs_add_drv(&pcfs_drv);

    read_motors(ini);
    read_macros("/usd/macros.txt");
    read_motor_actions(ini);

    pros::delay(20);

    chassis.setPos(12, 36, 0);
    chassis.setMotors(motor_map["Left Drive"], motor_map["Right Drive"]);
    chassis.setSensors({{4, 3, 4}, false}, {{4, 1, 2}, true}, {20});
    chassis.setDimensions({AbstractMotor::gearset::green, 1.0}, 2.75, 10.0);
    chassis.setKinematics(30.7936 * 0.3048, 0);
    chassis.setOdomDimensions({4.75, 2.75}, {2.75, 2.75}, {4.75, 2.75});
    chassis.setGains(
                        {27.8, 0.0, 2.4},
                        {2.525, 0.0, 1.4},
                        {0.01, 0.0, 0.0}
    );

    chassis.setDrive((ini.get("Drive", "drive_mod", "linear") == "linear") ? linearModifier : (ini.get("Drive", "drive_mod", "") == "square") ? squareModifier : (ini.get("Drive", "drive_mod", "") == "cubic") ? cubicModifier : exponentialModifier,
                    std::stod(ini.get("Drive", "drive_scale", "0.0")));

    if(std::holds_alternative<IMUPtr>(chassis.getAngular())){
        std::get<IMUPtr>(chassis.getAngular())->reset();
        while(std::get<IMUPtr>(chassis.getAngular())->is_calibrating())
            pros::delay(5);
        std::get<IMUPtr>(chassis.getAngular())->set_heading(boundDeg((360 - chassis.getOrientation() * (180 / M_PI))));
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

 void printPos(void* motor_ptr){
    MotorGroupWrapper* motor = (MotorGroupWrapper*) motor_ptr;
    while(1){
        std::cout << std::to_string(motor->getPosition()) << "\n";
        pros::delay(20);
    }
 }

void autonomous() {
    control_start = pros::millis();
    chassis.runOdom();
    pros::Task printOdomTask(printOdomController);    

    // std::ifstream ifs("/usd/ramsete.txt");

    // RamsetePath p;
    // ifs >> p;
    // r_controller.run(p);
    
    std::cout << chassis.getMaxVelocity() << "\n";

    while(pros::competition::is_autonomous())
        pros::delay(20);

    // pros::Task printOdom(printOdomController, nullptr, "Odom Print Task");
    // pros::Task save_auton_task(save_auton, nullptr, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "Save Auton Task");

    // std::string auton_path = ini.get("Config", "curr_auton", "");
    // if(auton_path.empty())
    //     return;

    // std::ifstream auton(auton_path);
    // auton_display(auton);

    // Path p({{12, 36}, {30, 36}, {30, 64}});
    // p.setMinLookahead(6);
    // p.setMaxLookahead(11);
    // p.setReversed(false);
    // p.injectPoints(3);
    // p.smoothPath();

    // p_controller.run(p);
    // p_controller.waitUntilSettled();

    // chassis.turnToPoint({30, 90});
    // chassis.waitUntilSettled();

    // save_auton_task.notify();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
    control_start = pros::millis();
    Menu m(mainMenu);
    
    std::vector<pros::Task> controllerTasks(
        {
            {printTargetController},
            {printOdomController},
            {printEncodersController}
        }
    );
    for(auto& task : controllerTasks)
        task.suspend();
    controller.clear();
    controllerTasks[0].resume();
    
    bool intakeBool = false;
    bool intakeRev = false;
    bool autoFly = true;
    int controllerMenu = 0;
    chassis.runOdom();

    while(1){
        if(controller[ControllerDigital::left].changedToPressed() && controllerMenu > 0){
            controllerTasks[controllerMenu].suspend();
            controller.clear();
            pros::delay(51);
            controllerTasks[--controllerMenu].resume();
        }
        else if(controller[ControllerDigital::right].changedToPressed() && controllerMenu < controllerTasks.size() - 1){
            controllerTasks[controllerMenu].suspend();
            controller.clear();
            pros::delay(51);
            controllerTasks[++controllerMenu].resume();
        }
        
        if(m.getFreeze()){
            chassis.arcade(0, 0);
            for(auto& m : motor_map)
                m.second.moveVoltage(0);
        }

        else{
            chassis.arcade(c_w.getAnalog(ControllerAnalog::leftY), c_w.getAnalog(ControllerAnalog::rightX));
            // run_actions(motor_controller_map);
            if(controller[ControllerDigital::L1].changedToPressed())
                macro_map["Drive Test"].run();
            // if(c_w.toggled(ControllerDigital::A))
            //     intakeBool = !intakeBool;
            // if(intakeBool && c_w.isPressed(ControllerDigital::B))
            //     intakeRev = true;
            // else
            //     intakeRev = false;
            // if(c_w.toggled(ControllerDigital::L1))
            //     launcher.launch();
            // if(c_w.toggled(ControllerDigital::L2))
            //     autoFly = !autoFly;
            // if(c_w.toggled(ControllerDigital::R1))
            //     launcher.getAngleChanger().set_value(true);
            // if(c_w.toggled(ControllerDigital::R2))
            //     launcher.getAngleChanger().set_value(false);

            // if(intakeRev)
            //     intake.moveVoltage(-12000);
            // else if(intakeBool)
            //     intake.moveVoltage(12000);
            // else
            //     intake.moveVoltage(0);
        }
        pros::delay(20);
    }

}