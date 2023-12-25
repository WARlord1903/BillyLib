#include "OdomChassis.hpp"
#include "MathUtils/Point2D.hpp"

double ptToPtDistance(const Point2D& p1, const Point2D& p2) { return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)); }
Point2D operator+(Point2D& lhs, Point2D& rhs) { return lhs += rhs; }
Point2D operator*(Point2D& lhs, Point2D& rhs) { return lhs *= rhs; }
Point2D operator*(Point2D& lhs, double rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
std::ostream& operator<<(std::ostream& os, const Point2D& p){ os << p.x << ", " << p.y; return os; }

Point2D strToPoint(std::string s) {
    double x;
    double y;
    std::string delim = ", ";
    x = std::stod(s.substr(0, s.find(delim)));
    s = s.erase(0, s.find(delim) + delim.length());
    y = std::stod(s);
    return {x, y};
}

double inchesToPixels(double in){
    return in * (240. / 144);
}

lv_point_t inchesToPixels(double x, double y){
    return {(lv_coord_t) (((240. / 144) * x)), (lv_coord_t) (((240. / 144) * y))};
}

lv_point_t odomToPixels(OdomChassis& c){
    lv_point_t res = inchesToPixels(c.getX(), 144. - c.getY());
    res.x += LV_HOR_RES / 4;
    return res;
}

Point2D pixelsToOdom(lv_coord_t x, lv_coord_t y){
    return {(144. / 240) * x, ((144. / 240) * y)};
}