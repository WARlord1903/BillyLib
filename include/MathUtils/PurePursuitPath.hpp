#ifndef PATH_HPP
#define PATH_HPP

#include "main.h"
#include "MathUtils/Point2D.hpp"
#include "MathUtils/Curve.hpp"

class PurePursuitPath : public Curve{
public:
    PurePursuitPath(): Curve{} {}
    PurePursuitPath(std::initializer_list<Point2D> ipoints, bool rev = false, double iminL = 3, double imaxL = 24, double ireroute_dist = 0):
        Curve{ipoints}, reverse{rev}, minL{iminL}, maxL{imaxL}, reroute_dist{ireroute_dist} {}
    PurePursuitPath(PurePursuitPath& p): reverse{p.reverse}, minL{p.minL}, maxL{p.maxL}, reroute_dist{p.reroute_dist}, reroute_bounds{p.reroute_bounds} { points = p.points; old_points = p.old_points; }
    void operator=(PurePursuitPath& p) { points = p.points; old_points = p.old_points; reverse = p.reverse; minL = p.minL; maxL = p.maxL; spacing = p.spacing; smoothing = p.smoothing; reroute_dist = p.reroute_dist; reroute_bounds = p.reroute_bounds; }
    double getMinLookahead() const { return minL; }
    double getMaxLookahead() const { return maxL; }
    void setMinLookahead(double m) { minL = m; }
    void setMaxLookahead(double m) { maxL = m; }
    bool getReversed() const { return reverse; }
    void setReversed(bool r) { reverse = r; }
    double getReroute() { return reroute_dist; }
    void setReroute(double r) { reroute_dist = r; }
    std::array<double, 4>& getBounds() { return reroute_bounds; }
    void setBounds(std::array<double, 4> ibounds) { reroute_bounds = ibounds; }
    void reroute(Point2D, double, double);
    std::string to_string() const override { 
        std::stringstream ret; 
        ret << "PATH_BEGIN\n" << spacing << "\n" << smoothing << "\n" << "OLD_PATH_BEGIN\n"; 
        for(auto& p : old_points) 
            ret << p << "\n";
        ret << "OLD_PATH_END\n" << minL << "\n" << maxL  << "\n" << reverse << "\n" << reroute_dist << "\n"
            << reroute_bounds[0] << "\n" << reroute_bounds[1] << "\n" << reroute_bounds[2] << "\n" << reroute_bounds[3] << "\n";
        for(auto& p : points) 
            ret << p << "\n";
        ret << "PATH_END\n"; 
        return ret.str(); 
    }
    std::istream& operator>>(std::istream&) override;
private:
    bool reverse;
    double reroute_dist;
    std::array<double, 4> reroute_bounds;
    double minL;
    double maxL;
    double spacing;
    double smoothing;
};

#endif