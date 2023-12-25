#ifndef RAMSETEPATH_HPP
#define RAMSETEPATH_HPP

#include "MathUtils/MathUtils.hpp"
#include "Point2D.hpp"
#include "main.h"
#include "MathUtils/Curve.hpp"

class RamsetePath : public Curve {
public:
    RamsetePath();
    RamsetePath(std::initializer_list<Point2D>, double, double, double, double = 0.1, double = 2.0, double = 0.7);
    void operator=(const RamsetePath& p) { points = p.points; old_points = p.old_points; spacing = p.spacing; smoothing = p.smoothing;  }
    void injectPoints(double = 6.0) override;
    void smoothPath(double = 0.78, double = 0.001) override;
    void update_vels(double);
    void push_back(Point2D p, bool update) { Curve::push_back(p); if(update) update_vels(maxVel); }
    void pop_back(bool update) { Curve::pop_back(); if(update) update_vels(maxVel); }
    void push_back_orig(Point2D p, bool update) { Curve::push_back_orig(p); if(update) update_vels(maxVel); }
    void pop_back_orig(bool update) { Curve::pop_back_orig(); if(update) update_vels(maxVel); }
    void setB(double ib) { b = ib; }
    double getB() { return b; }
    void setZeta(double izeta) { zeta = izeta; }
    double getZeta() { return zeta; }
    void setMaxVelocity(double iv) { maxVel = iv; }
    double getMaxVelocity() { return maxVel; }
    void setIntensity(double i) { intensity = i; }
    double getIntensity() { return intensity; }
    std::array<double, 5> operator[](unsigned i) { return {(points)[i].x, (points)[i].y, angles[i], linear_vels[i], angular_vels[i]}; }
    std::string to_string() const override {
        std::stringstream ret;
        ret << "RAMSETE_BEGIN\n" << spacing << "\n" << smoothing << "\n" << maxVel << "\n" << b << "\n" << zeta << "\n" << intensity << "\n" << "OLD_RAMSETE_BEGIN\n"; 
        for(auto& p : old_points) 
            ret << p << "\n";
        ret << "OLD_RAMSETE_END\n";
        for(auto& p : points) 
            ret << p << "\n";
        ret << "RAMSETE_POINTS_END\n";
        ret << boundRad(start_angle) << ", " << 0 << ", " << 0 << "\n";
        for(int i = 1; i < angles.size() - 1; i++)
            ret << boundRad(angles[i]) << ", " << linear_vels[i] << ", " << angular_vels[i] << "\n";
        ret << boundRad(end_angle) << ", " << 0 << ", " << 0 << "\n";
        ret << "RAMSETE_END\n"; 
        return ret.str(); 
    }

    std::istream& operator>>(std::istream&) override;
private:
    double start_angle;
    double end_angle;
    double b;
    double zeta;
    double maxVel;
    std::deque<double> angles;
    std::deque<double> linear_vels;
    std::deque<double> angular_vels;
    double intensity;
};

#endif