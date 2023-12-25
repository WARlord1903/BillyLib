#include "MathUtils/MathUtils.hpp"
#include "Globals.hpp"

double boundRad(double val){
    val = std::fmod(val, 2.0 * M_PI);
    while(val < 0)
        val += (2.0 * M_PI);
    return val;
}

double boundDeg(double deg){
    deg = std::fmod(deg, 360);
    while(deg < 0)
        deg += 360;
    return deg;
}

double getDelta(double last, double curr, double diam){
    return ((curr - last) * (M_PI * diam / 360));
}

double quadraticFormulaLargest(double a, double b, double c){
    if((b * b) < (4 * a * c))
        return INT32_MIN;
    if((-b + std::sqrt((b * b) - (4 * a * c))) / (2 * a) > (-b - std::sqrt((b * b) - (4 * a * c))) / (2 * a))
        return (-b + std::sqrt((b * b) - (4 * a * c))) / (2 * a);
    else
        return (-b - std::sqrt((b * b) - (4 * a * c))) / (2 * a);
}


double changeBase(double x, double y){
    return std::log2(x) / std::log2(y);
}

std::int32_t logGenerator(std::uint32_t x, std::uint32_t time, std::int32_t start, std::int32_t target){
    if(x == 0)
        return start;
    if(x > time)
        return target;
    return ((target - start) * changeBase(x, time)) + start;
}

double weightedAvg(double first, double second, double pct){
  return (first * (1. - pct)) + (second * pct);
}

std::int32_t calcVelocity(double distFromGoal) {
  double r = 1 - ((int) std::ceil(distFromGoal) - distFromGoal);
  int closestDist = (int) distFromGoal;
  if(r == 1.0)
    return dists[closestDist].first;
  else
    return  weightedAvg(dists[closestDist].first, dists[closestDist + 1].first, r);
}

std::int8_t sign(double n) { return (n >= 0) ? 1 : -1; }

double findMinAngle(double absTargetAngle, double currentHeading){
    double minAngle = absTargetAngle - currentHeading;
    if(minAngle > 180 || minAngle < -180)
        minAngle = -1 * sign(minAngle) * (360 - std::abs(minAngle));
    return minAngle;
}