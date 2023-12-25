#include "MotionControllers/MotionProfile.hpp"
#include "Globals.hpp"


void read_profiles(Ini_file& ini){
    for(auto& s : ini.getKeys("Motion Profiles")){
        std::string path = ini.get("Motion Profiles", s, "");
        if(!path.empty())
            motion_profiles[s] = path;
    }
}

void write_profiles(Ini_file& ini){
    for(auto& pair : motion_profiles){
        ini.set("Motion Profiles", pair.first, pair.second.getPath());
    }
}

double get_curr_val(Curve& c, std::uint32_t timestamp, int& line_index){
    Line2D line(c[line_index], c[line_index+1]);
    Point2D pt((double) timestamp, line.getY(timestamp));
    while(!line.inBounds(pt)){
        line_index++;
        line.setPoints(c[line_index], c[line_index+1]);
        pt.y = line.getY(timestamp);
    }
    return pt.y;
}

void profileLoop(void *profile_ptr){
    std::uint32_t start = pros::millis();
    std::uint32_t timestamp;
    MotionProfile* profile = (MotionProfile*) profile_ptr;
    int line_index = 0;
    if(pros::Task::notify_take(true, 0)){
        start = pros::millis();
        line_index = 0;
    }
    while((timestamp = pros::millis() - start) < profile->getCurve().back().x){
        double voltage = std::ceil(get_curr_val(profile->getCurve(), timestamp, line_index));
        for(auto& m : profile->getMotors())
            m->moveVoltage(voltage);
        pros::delay(20);
    }
    for(auto& m : profile->getMotors())
        m->moveVoltage(profile->getCurve().back().y);
}