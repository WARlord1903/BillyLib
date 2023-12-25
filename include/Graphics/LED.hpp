#ifndef LED_HPP
#define LED_HPP

#include "display/lv_objx/lv_led.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"

class LED : public LVGLObject{
public:
    LED(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_led_create(par, cpy)} {}
    LED(LVGLObject& par): LED{par.get(), NULL} {}
    LED(LVGLObject& par, LED& cpy): LED{par.get(), cpy.get()} {}
    void setBrightness(std::uint8_t brightness) { lv_led_set_bright(obj, brightness); }
    void setState(bool state) { (state) ? lv_led_on(obj) : lv_led_off(obj); }
    void toggleState() { lv_led_toggle(obj); }
    std::uint8_t getBrightness() { return lv_led_get_bright(obj); }
};

#endif