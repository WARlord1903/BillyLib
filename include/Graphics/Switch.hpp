#ifndef SWITCH_HPP
#define SWITCH_HPP

#include "Graphics/LVGLObject.hpp"
#include "display/lv_objx/lv_sw.h"

class Switch : public LVGLObject{
public:
    Switch(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject{lv_sw_create(par, cpy)} {}
    Switch(LVGLObject& par): Switch{par.get(), NULL} {}
    Switch(LVGLObject& par, Switch& cpy): Switch{par.get(), cpy.get()} {}
    void setState(bool en) { (en) ? lv_sw_on(obj) : lv_sw_off(obj); }
    void toggle() { lv_sw_toggle(obj); }
    void setStateAnim(bool en) { (en) ? lv_sw_on_anim(obj) : lv_sw_off_anim(obj); }
    void toggleAnim() { lv_sw_toggle_anim(obj); }
    void setAction(lv_action_t action) { lv_sw_set_action(obj, action); }
    void setAnimTime(std::uint16_t time) { lv_sw_set_anim_time(obj, time); }
    bool getState() { return lv_sw_get_state(obj); }
    lv_action_t getAction() { return lv_sw_get_action(obj); }
    std::uint16_t getAnimTime() { return lv_sw_get_anim_time(obj); }
};

#endif