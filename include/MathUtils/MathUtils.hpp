#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include "main.h"
#include <cmath>

double boundRad(double);
double boundDeg(double);
double getDelta(double, double, double);
double quadraticFormulaLargest(double, double, double);
std::int32_t linearGenerator(std::uint32_t, std::uint32_t, std::int32_t, std::int32_t);
std::int32_t logGenerator(std::uint32_t, std::uint32_t, std::int32_t, std::int32_t);
double weightedAvg(double, double, double);
std::int32_t calcVelocity(double);
std::int8_t sign(double);
double findMinAngle(double, double);

#endif