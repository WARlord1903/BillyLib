#include "MotionControllers/PurePursuitController.hpp"
#include "Globals.hpp"
#include "okapi/impl/device/distanceSensor.hpp"
#include "types.hpp"
#include <iostream>


void pploop(void *pptr){
    PurePursuitController* p = (PurePursuitController*) pptr;
    p->getMutex().take();
    p->setFollowing(true);
    while(1){
        auto vels = p->getVelocities();
        p->getChassis().getLeft().moveVelocity(vels.first - vels.second);
        p->getChassis().getRight().moveVelocity(vels.first + vels.second);
        if(vels.first == 0 && vels.second == 0)
            break;
        pros::delay(20);
    }
    pros::delay(750);
    p->getMutex().give();
    p->setFollowing(false);
}

Point2D PurePursuitController::findGoal(double ilookahead){
    static int lastFoundIndex = 0;
    if(pros::Task::notify_take(true, 0))
        lastFoundIndex = 0;
    std::pair<Point2D, Point2D> pts;
    Point2D igoal(-999, -999);
    Point2D curr(c->getX(), c->getY());

    for(int i = lastFoundIndex; i < p->size() - 1; i++){
        double cx = curr.x;
        double cy = curr.y;
        double x1_offset = (*p)[i].x - cx;
        double y1_offset = (*p)[i].y - cy;
        double x2_offset = (*p)[i+1].x - cx;
        double y2_offset = (*p)[i+1].y - cy;

        double dx = x2_offset - x1_offset;
        double dy = y2_offset - y1_offset;
        double dr = std::sqrt(dx * dx + dy * dy);
        double D = x1_offset * y2_offset - x2_offset * y1_offset;
        double discriminant = ilookahead * ilookahead * dr * dr - D * D;

        if(discriminant >= 0){
            double i1x = (D * dy + sign(dy) * dx * std::sqrt(discriminant)) / (dr * dr);
            double i2x = (D * dy - sign(dy) * dx * std::sqrt(discriminant)) / (dr * dr);

            double i1y = (-D * dx + std::abs(dy) * std::sqrt(discriminant)) / (dr * dr);
            double i2y = (-D * dx - std::abs(dy) * std::sqrt(discriminant)) / (dr * dr);
            
            Point2D sol1{i1x + cx, i1y + cy};
            Point2D sol2{i2x + cx, i2y + cy};

            double minX = ((*p)[i].x < (*p)[i + 1].x) ? (*p)[i].x : (*p)[i + 1].x;
            double maxX = ((*p)[i].x > (*p)[i + 1].x) ? (*p)[i].x : (*p)[i + 1].x;
            double minY = ((*p)[i].y < (*p)[i + 1].y) ? (*p)[i].y : (*p)[i + 1].y;
            double maxY = ((*p)[i].y > (*p)[i + 1].y) ? (*p)[i].y : (*p)[i + 1].y;

            if((minX <= sol1.x && sol1.x <= maxX) && (minY <= sol1.y && sol1.y <= maxY)){
                if((minX <= sol2.x && sol2.x <= maxX) && (minY <= sol2.y && sol2.y <= maxY))
                    pts = {sol1, sol2};
                else
                    pts = {sol1, {-999, -999}};        
            }
            else if((minX <= sol2.x && sol2.x <= maxX) && (minY <= sol2.y && sol2.y <= maxY))
                pts = {{-999, -999}, sol2};
            else
                pts = {{-999, -999}, {-999, -999}};
            if(pts.first.x != -999 || pts.second.x != -999){
                if(ptToPtDistance(pts.first, (*p)[i+1]) < ptToPtDistance(pts.second, (*p)[i+1]))
                    igoal = pts.first;
                else
                    igoal = pts.second;
                if(ptToPtDistance(igoal, (*p)[i+1]) < ptToPtDistance(curr, (*p)[i+1])){
                    lastFoundIndex = i;
                    break;
                }
                else
                    lastFoundIndex = i+1;
            }
        }
    }
    if(igoal.x == -999)
        return (*p)[lastFoundIndex]; 
    return igoal;
}

std::pair<double, double> PurePursuitController::getVelocities(){
    if(distance && p->getReroute() != 0)
        if(distance->get() / 25.4 <= p->getReroute())
            p->reroute(c->getPoint(), c->getOrientation(), distance->get() / 25.4);

    std::int16_t left_voltage = c->getLeft().getVoltage();
    std::int16_t right_voltage = c->getRight().getVoltage();

    if(left_voltage == 0 || right_voltage == 0)
        lookahead = (p->getMinLookahead() + p->getMaxLookahead()) / 2.;
    else if(std::abs(left_voltage) > std::abs(right_voltage))
        lookahead = weightedAvg(p->getMaxLookahead(), p->getMinLookahead(), (double) (std::abs(left_voltage) - std::abs(right_voltage)) / std::abs(left_voltage));
    else
        lookahead = weightedAvg(p->getMaxLookahead(), p->getMinLookahead(), (double) (std::abs(right_voltage) - std::abs(left_voltage)) / std::abs(right_voltage));

    if(lookahead < p->getMinLookahead())
        lookahead = p->getMinLookahead();
    else if(lookahead > p->getMaxLookahead())
        lookahead = p->getMaxLookahead();

    static double prevLinError = 0.;
    static double prevTurnError = 0.;
    if(pros::Task::notify_take(true, 0)){
        prevLinError = 0.;
        prevTurnError = 0.;
        lookahead = p->getMinLookahead();
        task->notify();
    }

    Point2D pos({c->getX(), c->getY()});
    goal = findGoal(lookahead);

    if(goal.x == -999)
        return {0, 0};

    double linearError = ptToPtDistance(goal, pos);
    double linearDerivative = linearError - prevLinError;
    prevLinError = linearError;
  
    double curr = std::fmod(c->getOrientation() * (180 / M_PI) + 180, 360) - 180;
    while(curr < -180)
        curr += 360;

    double target = std::atan2(goal.y - pos.y, goal.x - pos.x) * (180 / M_PI);
    if(p->getReversed()){
        target -= 180;
        if(target < -180)
            target += 360;
    }

    double turnError = std::fmod(target - curr + 180, 360) - 180;
    if(turnError < -180)
        turnError += 360;

    double turnDerivative = turnError - prevTurnError;
    prevTurnError = turnError;

    double linearVelocity = std::get<0>(c->getDriveGains()) * linearError + std::get<2>(c->getDriveGains()) * linearDerivative;
    double turnVelocity = std::get<0>(c->getTurnGains()) * turnError + std::get<2>(c->getTurnGains()) * turnDerivative;
    if(ptToPtDistance(pos, p->back()) < p->getMaxLookahead()){
        double scale = weightedAvg(0, 1, ptToPtDistance(pos, p->back()) / lookahead);
        if(ptToPtDistance(pos, p->back()) <= 2)
            return {0, 0};
        if(p->getReversed())
            return {-linearVelocity, turnVelocity * scale};
        else
            return {linearVelocity, turnVelocity * scale};
    }
    else{
        if(p->getReversed())
            return {-linearVelocity, turnVelocity};
        else
            return {linearVelocity, turnVelocity};
    }
}

void PurePursuitController::run(PurePursuitPath& ip){
    p = &ip;
    distance = nullptr;
    task = std::make_unique<pros::Task>(pploop, this, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "Pure Pursuit Loop");
    task->notify();
}

void PurePursuitController::run(PurePursuitPath& ip, DistanceSensor& id){
    p = &ip;
    distance = &id;
    task = std::make_unique<pros::Task>(pploop, this, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "Pure Pursuit Loop");
    task->notify();
}