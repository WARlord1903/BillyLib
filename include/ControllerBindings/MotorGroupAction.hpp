#ifndef MOTORGROUPACTION_HPP
#define MOTORGROUPACTION_HPP

#include "MotorGroup.hpp"
#include "ini.hpp"

void read_motor_actions(Ini_file&);
void write_motor_actions(Ini_file&);

class MotorGroupAction{
public:
    MotorGroupAction(std::string);
    void run();
    void toggle();
    void stop();
    bool getToggle() { return toggle_state; }
    std::string getName() { return m_name; }
    std::string title() { return m_name + " at " + std::to_string(voltage) + " mv" + ((toggle_state) ? " (Toggle)" : ""); }
    std::string toString() { return m_name + ":" + std::to_string(voltage) + ":" + std::to_string(toggle_state); }
    std::int16_t getVoltage() { return voltage; }
    void setName(std::string);
    void setVoltage(std::int16_t ivoltage) { voltage = ivoltage; }
    void setToggle(bool t) { toggle_state = t; } 
    bool isRunning() { return running; }
private:
    MotorGroupWrapper* m;
    std::string m_name;
    std::int16_t voltage;
    bool toggle_state;
    bool running;
};

#endif