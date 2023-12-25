#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"

class Vector2D;

Vector2D operator+(Vector2D&, Vector2D&);
Vector2D operator-(Vector2D&, Vector2D&);
Vector2D operator*(Vector2D&, double);
double operator*(Vector2D&, Vector2D&);
Vector2D operator/(Vector2D&, double);

class Vector2D{
public:
    Vector2D(double ix, double iy): x{ix}, y{iy} {}
    Vector2D(Point2D& start, Point2D& end): x{end.x - start.x}, y{end.y - start.y} {}
    inline Vector2D& operator+=(Vector2D& rhs) { x += rhs.x; y += rhs.y; return *this; }
    inline Vector2D& operator-=(Vector2D& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    inline Vector2D& operator*=(double mult) { x *= mult; y *= mult; return *this; }
    inline double operator*=(Vector2D& rhs) { return x * rhs.x + y * rhs.y; }
    inline Vector2D& operator/=(double div) { x /= div; y /= div; return *this; }
    inline double getX() { return x; }
    inline double getY() { return y; }
    double getAngle() { return std::atan2(y, x); }
    double magnitude() { return std::sqrt((x * x) + (y * y)); }
    Vector2D& normalize() { *this /= magnitude(); return *this; }
private:
    double x;
    double y;
};

#endif