#ifndef CURVE_HPP
#define CURVE_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"

class Curve;

std::ostream& operator<<(std::ostream&, const Curve&);
std::istream& operator>>(std::istream&, Curve&);

class Curve{
public:
    Curve();
    Curve(std::initializer_list<Point2D>, double = 0, double = 0);
    Curve(const Curve& p): points{p.points}, old_points{p.old_points}, spacing{p.spacing}, smoothing{p.smoothing} {}
    void operator=(Curve& p) { points = p.points; old_points = p.old_points; spacing = p.spacing; smoothing = p.smoothing; }
    Point2D& operator[](unsigned n) { return (points)[n]; }
    bool operator==(Curve& p) { return this->to_string() == p.to_string(); }
    bool operator!=(Curve& p) { return this->to_string() != p.to_string(); }
    std::size_t size() { return points.size(); }
    std::deque<Point2D>::iterator begin() { return points.begin(); }
    const std::deque<Point2D>::const_iterator begin() const { return points.begin(); }
    std::deque<Point2D>::iterator end() { return points.end(); }
    const std::deque<Point2D>::const_iterator end() const { return points.end(); }
    Point2D& back() { return points.back(); }
    Point2D& back_orig() { return old_points.back(); }
    void push_back(Point2D p) { points.push_back(p); }
    void pop_back() { points.pop_back(); }
    void push_back_orig(Point2D p) { old_points.push_back(p); }
    void pop_back_orig() { old_points.pop_back(); }
    void splice(std::initializer_list<Curve>);
    void clear() { points.clear(); old_points.clear(); }
    void copy(Curve&);
    virtual void injectPoints(double = 6.0);
    virtual void smoothPath(double = 0.78, double = 0.001);
    void setSpacing(double ispacing) { spacing = ispacing; }
    double getSpacing() { return spacing; }
    void setSmoothing(double ismoothing) { smoothing = ismoothing; }
    virtual std::string to_string() const { 
        std::stringstream ret; 
        ret << "CURVE_BEGIN\n" << spacing << "\n" << smoothing << "\n" << "OLD_CURVE_BEGIN\n"; 
        for(auto& p : old_points) 
            ret << p << "\n";
        ret << "OLD_CURVE_END\n";
        for(auto& p : points) 
            ret << p << "\n";
        ret << "CURVE_END\n"; 
        return ret.str(); 
    }

    virtual std::istream& operator>>(std::istream&);
protected:
    std::deque<Point2D> points;
    std::deque<Point2D> old_points;
    double spacing;
    double smoothing;
};

#endif