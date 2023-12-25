#ifndef MENU_HPP
#define MENU_HPP

#include "main.h"

void runCallback(void*);

class Menu;

void mainMenu(Menu&, void*);

void controlMenu1(Menu&, void*);

void joystickModifierSelect(Menu&, void*);
void joystickModifier(Menu&, void*);

void macroEditor(Menu&, void*);

void buttonSelect(Menu&, void*);
void buttonActionMenu(Menu&, void*);
void newAction(Menu&, void*);
void editMotorAction(Menu&, void*);

void motorGroupMenu1(Menu&, void*);
void editMotorGroupMenu(Menu&, void*);
void editPortsMenu(Menu&, void*);
void addMotorGroupMenu(Menu&, void*);

void pathPlanner(Menu&, void*);

void autonSelect(Menu&, void*);
void odomVisualizer(Menu&, void*);

void wait_until_notified(Menu&);

class Menu{
private:
    using callback_t = void (*)(Menu&, void*);
    void* param;
    callback_t callback;
    pros::Task task;
    bool freeze;
public:
    Menu(callback_t t = nullptr, void *iparam = nullptr): task{runCallback, this, "Menu Task"} { setCallback(t, iparam); }
    void setCallback(callback_t t, void* iparam = nullptr) { lv_obj_clean(lv_scr_act()); callback = t; param = iparam; task.notify(); }
    callback_t getCallback() { return callback; }
    void* getParam() { return param; }
    bool getFreeze() { return freeze; }
    void setFreeze(bool f) { freeze = f; }
    pros::Task& getTask() { return task; }
};

std::string button_matrix_page(Menu&, std::vector<std::string>&, bool = true, unsigned = 0);
std::string button_matrix_page(Menu&, std::initializer_list<std::string>, bool = true, unsigned = 0);

std::string keyboard_menu(Menu&, bool = false);

double spinbox(Menu&, double = 0., unsigned = 10, unsigned = 5, double = -INFINITY, double = INFINITY, std::string = "");
int spinbox(Menu&, int = 0, unsigned = 5, int = -99999, int = 99999, std::string = "");

void auton_display(std::istream&);
void auton_display(std::string);
#endif