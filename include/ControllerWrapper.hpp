#ifndef CONTROLLERWRAPPER_HPP
#define CONTROLLERWRAPPER_HPP

#include "main.h"
#include "SerialWrapper.hpp"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <string>
#include <variant>

void read(void*);

class ControllerWrapper{
public:
    ControllerWrapper(): c{}, e{nullptr} {}
    ControllerWrapper(std::uint8_t port):c {}, e{std::make_unique<ExControllerData>(port)} {  }

    std::uint16_t getMax() { if(e) return e->max; else return 255; }
    double getAnalog(ControllerAnalog ichannel) {
        if(e && c.getAnalog(ichannel) == 0)
            return e->getAnalog(ichannel);
        else
            return c.getAnalog(ichannel);
    }
    bool isPressed(ControllerDigital button){
        bool p = false;
        if(e)
            p = e->pressed[(int) button - 6];
        return p || c[button].isPressed();
    }
    bool toggled(ControllerDigital button){
        bool temp = false;
        if(e){
            bool temp = e->pressed[(int) button - 6] && !e->last_pressed[(int) button - 6];
            e->last_pressed[(int) button - 6] = e->pressed[(int) button - 6];
        }
        return temp || c[button].changedToPressed();
    }
    struct ExControllerData{
        ExControllerData(std::uint8_t port): datastream{std::make_unique<SerialWrapper>(port)}, update{read, this, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "Controller Task"} {}
        void process_command();
        double getAnalog(ControllerAnalog analog){ return (analog == ControllerAnalog::leftX) ? left_x : (analog == ControllerAnalog::leftY) ? left_y : (analog == ControllerAnalog::rightX) ? right_x : right_y; };
        double left_x;
        double left_y;
        double right_x;
        double right_y;
        std::array<bool, 12> pressed;
        std::array<bool, 12> last_pressed;
        std::uint16_t max;
        std::unique_ptr<SerialWrapper> datastream;
        pros::Task update;
    };
private:
    Controller c;
    std::unique_ptr<ExControllerData> e;
};

#endif