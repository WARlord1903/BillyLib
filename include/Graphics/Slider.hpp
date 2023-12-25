#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "Graphics/LVGLObject.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_slider.h"

class Slider : public LVGLObject{
public:
    Slider(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject(lv_slider_create(par, cpy)) {}
    Slider(LVGLObject& par): Slider(par.get()) {}
    Slider(LVGLObject& par, Slider& cpy): Slider(par.get(), cpy.get()) {}
    std::int16_t getValue() { return lv_slider_get_value(obj); }
    std::int16_t getMin() { return lv_slider_get_min_value(obj); }
    std::int16_t getMax() { return lv_slider_get_max_value(obj); }
    void setValue(std::int16_t value) { lv_slider_set_value(obj, value); }
    void setRange(std::int16_t min, std::int16_t max) { lv_slider_set_range(obj, min, max); }
};

#endif