#include "main.h"
#include "MathUtils/Point2D.hpp"
#include "MathUtils/Circle.hpp"
#include "MathUtils/Vector2D.hpp"
#include "MathUtils/PurePursuitPath.hpp"
#include "Globals.hpp"
#include <string>

std::ostream& operator<<(std::ostream& os, const PurePursuitPath& p){
    os << p.to_string();
    return os;
}

std::istream& PurePursuitPath::operator>>(std::istream& is){
    this->clear();
    std::string temp;
    std::getline(is, temp);
    if(temp == "PATH_BEGIN")
        std::getline(is, temp);
    this->setSpacing(std::stod(temp));
    std::getline(is, temp);
    this->setSmoothing(std::stod(temp));
    std::getline(is, temp);
    if(temp == "OLD_PATH_BEGIN")
        while(std::getline(is, temp) && temp != "OLD_PATH_END")
            this->push_back_orig(strToPoint(temp));
    std::getline(is, temp);
    this->setMinLookahead(std::stod(temp));
    std::getline(is, temp);
    this->setMaxLookahead(std::stod(temp));
    std::getline(is, temp);
    this->setReversed(std::stoi(temp));
    std::getline(is, temp);
    this->setReroute(std::stod(temp));
    std::array<double, 4> bounds;
    for(int i = 0; i < bounds.size(); i++){
        std::getline(is, temp);
        bounds[i] = std::stod(temp);
    }
    this->setBounds(bounds);
    while(std::getline(is, temp) && temp != "PATH_END")
        this->push_back(strToPoint(temp));
    return is;
}

void PurePursuitPath::reroute(Point2D curr, double angle, double distance){
    const Point2D start{-999, -999};
    static Circle last_route{start, 0};
    static Circle new_route{start, 0};
    if(ptToPtDistance(last_route.getCenter(), new_route.getCenter()) > distance || last_route.getCenter() == start && pros::millis() - control_start > 750){
        Point2D center =  
        {
            ((reverse) ? curr.x + std::cos(boundRad(M_PI - angle)) * distance : curr.x + std::cos(boundRad(angle)) * distance),
            ((reverse) ? curr.y + std::sin(boundRad(M_PI - angle)) * distance : curr.y + std::sin(boundRad(angle)) * distance)
        };

        new_route.setCenter(center);
        new_route.setRadius(distance);

        PurePursuitPath old_points;

        bool found = false;
        for(auto& p : points){
            if((found && !new_route.inCircle(p)) || points.size() == 1)
                break;
            else if(new_route.inCircle(p) && points.size() > 1){
                found = true;
                points.pop_front();
            }
            else if(!found){
                old_points.push_back(points.at(0));
                points.pop_front();
            }
        }
        PurePursuitPath p1;
        PurePursuitPath p2;
        //var named "start" above. Change?
        Point2D start;

        if(!reverse)
            start = new_route.getPoint(boundRad(M_PI - angle));
        else
            start = new_route.getPoint(boundRad(angle));
        Circle new_points(start, spacing);
        while(ptToPtDistance(new_points.getCenter(), points.at(0)) > spacing * 2){
            auto pts = new_route.getIntersection(new_points);
            if(new_route.getRadians(pts.first) > new_route.getRadians(pts.second) || new_route.getRadians(new_points.getCenter()) - new_route.getRadians(pts.first) >= 0){
                p1.push_back(pts.first);
                new_points.setCenter(pts.first);
            }
            else{
                p1.push_back(pts.second);
                new_points.setCenter(pts.second);
            }
        }

        new_points.setCenter(start);
        while(ptToPtDistance(new_points.getCenter(), points.at(0)) > spacing * 2){
            auto pts = new_route.getIntersection(new_points);
            if(new_route.getRadians(pts.first) <= new_route.getRadians(pts.second) && !(new_route.getRadians(new_points.getCenter()) - new_route.getRadians(pts.first) >= 0)){
                p2.push_back(pts.first);
                new_points.setCenter(pts.first);
            }
            else{
                p2.push_back(pts.second);
                new_points.setCenter(pts.second);
            }
        }
        if(p1.size() < p2.size()){
            old_points.splice({p1, *this});
            points = old_points.points;
        }
        else{
            old_points.splice({p2, *this});
            points = old_points.points;
        }
        smoothPath(smoothing);

        last_route = new_route;
    }
}