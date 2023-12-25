#include "MathUtils/Circle.hpp"

std::pair<Point2D, Point2D> Circle::getIntersection(Circle& c){
    double dist = ptToPtDistance(center, c.center);
    if(dist > radius + c.radius)
        return {{-999, -999}, {-999, -999}};
    if(dist < std::abs(radius - c.radius))
        return {{-999, -999}, {-999, -999}};
    if(dist == 0 && radius == c.radius)
        return {{-999, -999}, {-999, -999}};
    double a = (radius * radius - c.radius * c.radius + dist * dist) / (2 * dist);
    double h = std::sqrt(radius * radius - a * a);
    double x2 = center.x + a * (c.center.x - center.x) / dist;   
    double y2 = center.y + a * (c.center.y - center.y) / dist;   
    double x3 = x2 + h * (c.center.y - center.y) / dist;
    double y3 = y2 - h * (c.center.x - center.x) / dist;
    double x4 = x2 - h * (c.center.y - center.y) / dist;
    double y4 = y2 + h * (c.center.x - center.x) / dist;
    return {{x3, y3}, {x4, y4}};
}

Circle::Circle(Point2D p1, Point2D p2, Point2D p3){
    double x12 = p1.x - p2.x;
    double x13 = p1.x - p3.x;

    double y12 = p1.y - p2.y;
    double y13 = p1.y - p3.y;

    double y31 = p3.y - p1.y;
    double y21 = p2.y - p1.y;
    
    double x31 = p3.x - p1.x;
    double x21 = p2.x - p1.x;

    double sx13 = p1.x * p1.x - p3.x * p3.x;
    double sy13 = p1.y * p1.y - p3.y * p3.y;
    double sx21 = p2.x * p2.x - p1.x * p1.x;
    double sy21 = p2.y * p2.y - p1.y * p1.y;

    double h = -(((sx13) * (y12)
             + (sy13) * (y12)
             + (sx21) * (y13)
             + (sy21) * (y13))
            / (2 * ((x31) * (y12) - (x21) * (y13))));

    double k = -(((sx13) * (x12)
             + (sy13) * (x12)
             + (sx21) * (x13)
             + (sy21) * (x13))
            / (2 * ((y31) * (x12) - (y21) * (x13))));

    double c = -(p1.x * p1.x) - p1.y * p1.y - 2 * -h * p1.x - 2 * -k * p1.y;

    center = {h, k};
    radius = std::sqrt(h * h + k * k - c);
}

bool Circle::inCircle(Point2D& p){
    return ptToPtDistance(p, center) < radius;
}
