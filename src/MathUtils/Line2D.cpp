#include "MathUtils/Line2D.hpp"


double Line2D::getY(double x){
    double slope = (p2.y - p1.y) / (p2.x - p1.x);
    double intercept = p1.y - slope * p1.x;
    return slope * x + intercept;
};

bool Line2D::inBounds(Point2D p){
    double min_x = (p1.x < p2.x) ? p1.x : p2.x;
    double max_x = (p1.x > p2.x) ? p1.x : p2.x;
    double min_y = (p1.y < p2.y) ? p1.y : p2.y;
    double max_y = (p1.y > p2.y) ? p1.y : p2.y;
    return p.x >= min_x - 0.01 && p.x <= max_x + 0.01 && p.y >= min_y - 0.01 && p.y <= max_y + 0.01;
}