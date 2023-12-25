#ifndef POINT2D_HPP
#define POINT2D_HPP

#include "main.h"

struct Point2D;

double ptToPtDistance(const Point2D&, const Point2D&);
Point2D pixelsToOdom(lv_coord_t, lv_coord_t);
Point2D operator+(Point2D&, Point2D&);
Point2D operator*(Point2D&, Point2D&);
Point2D operator*(Point2D&, double);

class OdomChassis;

double inchesToPixels(double);
lv_point_t inchesToPixels(double, double);
lv_point_t odomToPixels(OdomChassis&);

struct Point2D{
    Point2D() = default;
    Point2D(double ix, double iy): x{ix}, y{iy} {}
    double x;
    double y;
    void operator=(Point2D rhs) { x = rhs.x; y = rhs.y; }
    bool operator==(const Point2D& rhs) const { return x == rhs.x && y == rhs.y; }
    Point2D& operator+=(Point2D& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Point2D& operator*=(Point2D& rhs) { x *= rhs.x; y *= rhs.y; return *this; }};

std::ostream& operator<<(std::ostream&, const Point2D&);
Point2D strToPoint(std::string);

#endif