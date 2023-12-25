#include "MathUtils/RamsetePath.hpp"
#include "MathUtils/Vector2D.hpp"
#include "MathUtils/Circle.hpp"
#include "ini.hpp"
#include "pros/link.h"

RamsetePath::RamsetePath(): Curve() {
    angles.clear();
    linear_vels.clear();
    angular_vels.clear();
}

RamsetePath::RamsetePath(std::initializer_list<Point2D> ipoints, double imaxVel, double istart_angle, double iend_angle, double ib, double izeta, double iintensity): Curve(ipoints), maxVel{imaxVel}, start_angle{istart_angle}, end_angle{iend_angle}, b{ib}, zeta{izeta}, intensity{iintensity} {
    update_vels(maxVel);
}

std::istream& RamsetePath::operator>>(std::istream& is){
    clear();
    angles.clear();
    linear_vels.clear();
    angular_vels.clear();
    std::string temp;
    std::getline(is, temp);
    if(temp == "RAMSETE_BEGIN")
        std::getline(is, temp);
    this->setSpacing(std::stod(temp));
    std::getline(is, temp);
    this->setSmoothing(std::stod(temp));
    std::getline(is, temp);
    this->setMaxVelocity(std::stod(temp));
    std::getline(is, temp);
    this->setB(std::stod(temp));
    std::getline(is, temp);
    this->setZeta(std::stod(temp));
    std::getline(is, temp);
    this->setIntensity(std::stod(temp));
    std::getline(is, temp);
    if(temp == "OLD_RAMSETE_BEGIN")
        while(std::getline(is, temp) && temp != "OLD_RAMSETE_END")
            push_back_orig(strToPoint(temp), false);
    while(std::getline(is, temp) && temp != "RAMSETE_POINTS_END")
        push_back(strToPoint(temp), false);
    while(std::getline(is, temp) && temp != "RAMSETE_END"){
        auto vals = split_string(temp, ", ");
        angles.push_back(boundRad(std::stod(vals[0])));
        linear_vels.push_back(std::stod(vals[1]));
        angular_vels.push_back(std::stod(vals[2]));
    }
    this->start_angle = angles[0];
    this->end_angle = angles.back();
    return is;
}


void RamsetePath::update_vels(double imaxVel) {
    maxVel = imaxVel;
    angles.clear();
    linear_vels.clear();
    angular_vels.clear();
    angles.push_back(start_angle);
    linear_vels.push_back(0);
    angular_vels.push_back(0);
    for(int i = 1; i < points.size() - 1; i++){
        Circle c((points)[i-1], (points)[i], (points)[i+1]);
        Vector2D v1((points)[i], (points)[i+1]);
        Vector2D v2((points)[i-1], (points)[i]);
        angles.push_back(v1.getAngle());
        double deltaTheta;
        if(i > 1)
            deltaTheta = v1.getAngle() - v2.getAngle();
        else
            deltaTheta = v1.getAngle() - start_angle;
        double lin_vel = intensity / (1. / (c.getRadius() * 0.0254));
        if(lin_vel < -maxVel)
            lin_vel = -maxVel;
        else if(lin_vel > maxVel)
            lin_vel = maxVel;
        double deltaT = ((c.getRadius() * 0.0254) * deltaTheta) / lin_vel;
        if(deltaT > 145)
            deltaT = 0; 
        linear_vels.push_back(lin_vel);
        angular_vels.push_back(deltaTheta * deltaT);
    }
    angles.push_back(end_angle);
    linear_vels.push_back(0);
    angular_vels.push_back(0);
}

void RamsetePath::injectPoints(double ispacing){
    Curve::injectPoints(ispacing);
    update_vels(maxVel);
}

void RamsetePath::smoothPath(double weight_smooth, double tolerance){
    Curve::smoothPath(weight_smooth, tolerance);
    update_vels(maxVel);
}