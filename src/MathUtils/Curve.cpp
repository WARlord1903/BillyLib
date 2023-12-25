#include "main.h"
#include "MathUtils/Curve.hpp"
#include "MathUtils/Vector2D.hpp"

Curve::Curve(): 
    points{{}}, 
    old_points{{}},
    spacing{0},
    smoothing{0}
    { 
        if(!points.empty()) 
            points.clear();
        if(!old_points.empty()) 
            old_points.clear(); 
    }

Curve::Curve(std::initializer_list<Point2D> ipoints, double ispacing, double ismoothing): 
    points{ipoints.begin(), ipoints.end()}, 
    old_points{ipoints.begin(), ipoints.end()}, 
    spacing{ispacing},
    smoothing{ismoothing}
    { injectPoints(spacing); smoothPath(smoothing); }
    
void Curve::copy(Curve& p) { 
    points.clear(); 
    old_points.clear(); 
    std::copy(p.points.begin(), p.points.end(), std::back_inserter(points)); 
    std::copy(p.old_points.begin(), p.old_points.end(), std::back_inserter(old_points)); 
}

void Curve::splice(std::initializer_list<Curve> paths) { 
    for(auto& p : paths){ 
        points.insert(points.end(), p.points.begin(), p.points.end());
        old_points.insert(old_points.end(), p.old_points.begin(), p.old_points.end()); 
    }
}

void Curve::injectPoints(double ispacing){
    spacing = ispacing;
    if(spacing == 0)
        return;
    points.clear();
    points.push_back((old_points)[0]);
    for(int i = 0; i < old_points.size() - 1; i++){
        Point2D start = ((old_points)[i]);
        Vector2D vec(start, (old_points)[i+1]);
        int numPoints = std::ceil(vec.magnitude() / ispacing);
        Vector2D vecNorm = vec.normalize() * ispacing;
        for(int j = 1; j < numPoints; j++){
            points.push_back({start.x + vecNorm.getX() * j, start.y + vecNorm.getY() * j});
        }
        points.push_back((old_points)[i+1]);
    }
}

void Curve::smoothPath(double weight_smooth, double tolerance){

    smoothing = weight_smooth;

    std::deque<Point2D> newPath;

    for(auto p : points)
        newPath.push_back(p);

    double change = tolerance;

    while(change >= tolerance){
        change = 0.0;
        for(int i = 1; i < points.size() - 1; i++){
            for(int j = 0; j < 2; j++){
                if(j == 0){
                    double aux = newPath[i].x;
                    newPath[i].x += ((1 - weight_smooth) * ((points)[i].x - newPath[i].x) + weight_smooth * (newPath[i-1].x + newPath[i+1].x - (2.0 * newPath[i].x)));
                    change += std::abs(aux - newPath[i].x);
                }
                if(j == 1){
                    double aux = newPath[i].y;
                    newPath[i].y += ((1 - weight_smooth) * ((points)[i].y - newPath[i].y) + weight_smooth * (newPath[i-1].y + newPath[i+1].y - (2.0 * newPath[i].y)));
                    change += std::abs(aux - newPath[i].y);
                }
            }
        }
    }
    points.clear();
    for(auto p : newPath)
        points.push_back(p);
}

std::istream& Curve::operator>>(std::istream &is){
    this->clear();
    std::string temp;
    std::getline(is, temp);
    if(temp == "CURVE_BEGIN")
        std::getline(is, temp);
    this->setSpacing(std::stod(temp));
    std::getline(is, temp);
    this->setSmoothing(std::stod(temp));
    std::getline(is, temp);
    if(temp == "OLD_CURVE_BEGIN")
        while(std::getline(is, temp) && temp != "OLD_CURVE_END")
            this->push_back_orig(strToPoint(temp));
    while(std::getline(is, temp) && temp != "CURVE_END")
        this->push_back(strToPoint(temp));
    return is;
}

std::ostream& operator<<(std::ostream& os, const Curve& p){
    os << p.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, Curve& p){
    return p.operator>>(is);
}