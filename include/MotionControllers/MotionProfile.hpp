#ifndef MOTIONPROFILE_HPP
#define MOTIONPROFILE_HPP

#include "main.h"
#include "ini.hpp"
#include "MotorGroup.hpp"
#include "MathUtils/Curve.hpp"
#include "MathUtils/Line2D.hpp"
#include <string>

void read_profiles(Ini_file&);
void write_profiles(Ini_file&);

void profileLoop(void*);

class MotionProfile {
public:
    MotionProfile() = default;
    MotionProfile(std::string ipath) { 
        path = ipath; 
        std::ifstream stream(ipath); 
        stream >> c;
    }
    MotionProfile(const MotionProfile& rhs): c{rhs.c}, m{rhs.m}, path{rhs.path} {}
    void operator=(std::string ipath) { path = ipath; std::ifstream stream(ipath); stream >> c; }
    std::vector<MotorGroupWrapper*>& getMotors() { return m; }
    pros::Task& getTask() { return *task; }
    Curve& getCurve() { return c; }
    std::string& getPath() { return path; }
    void run(MotorGroupWrapper& im) { m.clear(); m.push_back(&im); task = std::make_unique<pros::Task>(profileLoop, this, "Motion Profile Task"); task->notify(); }
    void run(std::initializer_list<MotorGroupWrapper*> im) { m.clear(); for(auto motor : im) m.push_back(motor); task = std::make_unique<pros::Task>(profileLoop, this, "Motion Profile Task"); task->notify(); }
private:
    std::vector<MotorGroupWrapper*> m;
    Curve c;
    std::unique_ptr<pros::Task> task;
    std::string path;
};

#endif