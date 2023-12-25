#include "Globals.hpp"
#include "MotionControllers/TurnPIDController.hpp"
#include "MotionControllers/DrivePIDController.hpp"
#include "main.h"
#include "OdomChassis.hpp"
#include "MathUtils/Point2D.hpp"
#include "MotionControllers/DistancePIDController.hpp"
#include "pros/distance.hpp"
#include <cmath>
#include <memory>
#include <variant>

double linearModifier(double val){
    return val * 12000;
}

double exponentialModifier(double val){
    return (val >= 0) ? std::pow(12000, val) : -std::pow(12000, std::abs(val));
}

double squareModifier(double val){
    return std::pow(109.544512 * val, 2);
}

double cubicModifier(double val){
    return std::pow(22.8942849 * val, 3);
}

void OdomChassis::OdomChassisImpl::tank(double left, double right){
    static bool left_counted = false;
    static bool right_counted = false;
    if(left != 0 && !left_counted){
        left_counted = true;
        motor_count[left_motors++];
        left_motors->moveVoltage(weightedAvg(linearModifier(left), mod(left), scale));
    }
    else if(left != 0 && left_counted)
        left_motors->moveVoltage(weightedAvg(linearModifier(left), mod(left), scale));
    else{
        left_counted = false;
        if(motor_count[left_motors] == 0)
            left_motors->moveVelocity(0);
        else
            motor_count[left_motors]--;
    }
    if(right != 0 && !right_counted){
        right_counted = true;
        motor_count[right_motors++];
        right_motors->moveVoltage(weightedAvg(linearModifier(right), mod(right), scale));
    }
    else if(right != 0 && right_counted)
        right_motors->moveVoltage(weightedAvg(linearModifier(right), mod(right), scale));
    else{
        right_counted = false;
        if(motor_count[right_motors] == 0)
            right_motors->moveVelocity(0);
        else
            motor_count[right_motors]--;
    }
}

void OdomChassis::OdomChassisImpl::arcade(double steer, double turn){
    static bool counted = false;
    if((steer != 0 || turn != 0)){
        if(!counted){
            counted = true;
            motor_count[left_motors]++;
            motor_count[right_motors]++;
        }
        left_motors->moveVoltage(weightedAvg(linearModifier(steer), mod(steer), scale) + weightedAvg(linearModifier(turn), mod(turn), scale));
        right_motors->moveVoltage(weightedAvg(linearModifier(steer), mod(steer), scale) - weightedAvg(linearModifier(turn), mod(turn), scale));
    }
    else if(counted){
        counted = false;
        motor_count[left_motors]--;
        motor_count[right_motors]--;
        if(motor_count[left_motors] == 0)
            left_motors->moveVoltage(0);
        if(motor_count[right_motors] == 0)
            right_motors->moveVoltage(0);
    }
}

void OdomChassis::moveToPointAsync(Point2D p){
    impl->odom = std::make_unique<DrivePIDController>(std::get<0>(chassis.getDriveGains()), std::get<1>(chassis.getDriveGains()), std::get<2>(chassis.getDriveGains()), *this);
    impl->odom->run(p);
}

void OdomChassis::moveToPoint(Point2D p){
    moveToPointAsync(p);
    waitUntilSettled();
}

void OdomChassis::turnToPointAsync(Point2D p){
    impl->odom = std::make_unique<TurnPIDController>(std::get<0>(chassis.getTurnGains()), std::get<1>(chassis.getTurnGains()), std::get<2>(chassis.getTurnGains()), *this);
    impl->odom->run(p);
}

void OdomChassis::turnToPoint(Point2D p){
    turnToPointAsync(p);
    waitUntilSettled();
}

void OdomChassis::driveToPoint(Point2D p){
    turnToPoint(p);
    moveToPoint(p);
}

void OdomChassis::driveToPointAsync(Point2D p){
    turnToPoint(p);
    moveToPointAsync(p);
}

void OdomChassis::waitUntilSettled(){
    pros::delay(50);
    if(impl->odom){
        impl->odom->getMutex().take();
        impl->odom->getMutex().give();
    }
}

void OdomChassis::OdomChassisImpl::update(){
    static double prevL = 0;
    static double prevR = 0;
    static double prevM = 0;
    static double prevT = boundRad(pos.orientation);

    double L = getLeftEncoder().get();
    double M = getMidEncoder().get();
    double R = 0.0;

    if(std::holds_alternative<ADIEncoderPtr>(getAngular()))
        R = std::get<ADIEncoderPtr>(getAngular())->get();

    double deltaL = getDelta(prevL, L, getLeftDiameter());
    double deltaM = getDelta(prevM, M, getMidDiameter());
    double deltaR = 0.0;

    if(std::holds_alternative<ADIEncoderPtr>(getAngular()))
        deltaR = getDelta(prevR, R, getRightDiameter());

    prevL = L;
    prevR = R;
    prevM = M;

    double deltaT = 0.0;
        
    if(std::holds_alternative<IMUPtr>(getAngular())){
        pos.orientation = boundRad((2 * M_PI - std::get<IMUPtr>(getAngular())->get_heading() * (M_PI / 180.)));
        deltaT = pos.orientation - prevT;
    }
    else
        deltaT = boundRad((deltaL - deltaR) / (getLeftDistance() + getRightDistance()));
        
    prevT = boundRad(pos.orientation);

    double localX = 0.0;
    double localY = 0.0;

    if(deltaT == 0){
        localX = deltaM;
        localY = deltaL;
    }
    else{
        localX = 2.0 * std::sin(deltaT / 2.0) * ((deltaM / deltaT) + getMidDistance());
        localY = 2.0 * std::sin(deltaT / 2.0) * ((deltaL / deltaT) + getLeftDistance());
    }

    double averageOrientation = pos.orientation - (deltaT / 2.0);

    pos.x += ((localX * std::sin(-averageOrientation)) + (localY * std::cos(-averageOrientation)));
    pos.y += ((localX * std::cos(-averageOrientation)) - (localY * std::sin(-averageOrientation)));
}

void odomLoop(void *chassis_ptr){
    OdomChassis *local_chassis = (OdomChassis*) chassis_ptr;
    while(1){
        local_chassis->update();
        pros::delay(20);
    }
}