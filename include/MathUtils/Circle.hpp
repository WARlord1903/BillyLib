#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"
#include "MathUtils/MathUtils.hpp"

class Circle{
public:
    Circle(Point2D icenter, double iradius): center{icenter}, radius{iradius} {} 
    Circle(Point2D, Point2D, Point2D);
    Point2D getPoint(double angle) { return {center.x + radius * std::cos(angle), center.y + radius * std::sin(angle)}; }
    std::pair<Point2D, Point2D> getIntersection(Circle&);
    double getRadius() { return radius; }
    const Point2D& getCenter() { return center; }
    void setCenter(Point2D c) { center = c; }
    void setRadius(double r) { radius = r; }
    double getRadians(Point2D p) { return boundRad(std::atan2(center.y - p.y, center.x - p.x)); }
    double getDegrees(Point2D p) { return getRadians(p) * 180. / M_PI; }
    bool inCircle(Point2D&);
private:
    Point2D center;
    double radius;
};

#endif