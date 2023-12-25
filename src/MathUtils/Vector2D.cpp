#include "MathUtils/Vector2D.hpp"

Vector2D operator+(Vector2D& lhs, Vector2D& rhs) { return lhs += rhs; }
Vector2D operator-(Vector2D& lhs, Vector2D& rhs) { return lhs -= rhs; }
Vector2D operator*(Vector2D& lhs, double rhs) { return lhs *= rhs; }
double operator*(Vector2D& lhs, Vector2D& rhs) { return lhs *= rhs; }
Vector2D operator/(Vector2D& lhs, double rhs) { return lhs /= rhs; }