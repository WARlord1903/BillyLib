#ifndef LINE2D_HPP
#define LINE2D_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"
#include "MathUtils/Vector2D.hpp"

class Line2D {
public:
    Line2D() = default;
    Line2D(Point2D ip1, Point2D ip2): p1{ip1}, p2{ip2} {}
    Line2D(const Line2D& rhs): p1{rhs.p1}, p2{rhs.p2} {}
    double getY(double);
    bool inBounds(Point2D);
    void setFirstPoint(Point2D ip1) { p1 = ip1; }
    void setSecondPoint(Point2D ip2) { p2 = ip2; }
    void setPoints(Point2D ip1, Point2D ip2) { p1 = ip1; p2 = ip2; }
private:
    Point2D p1;
    Point2D p2;
};

#endif