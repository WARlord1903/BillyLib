#ifndef ARC_HPP
#define ARC_HPP

#include "main.h"
#include "Graphics/LVGLObject.hpp"

class Arc : public LVGLObject{
public:
    Arc(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject{lv_arc_create(par, cpy)} {}
    Arc(LVGLObject &par): Arc{par.get(), NULL} {}
    Arc(LVGLObject &par, Arc& cpy): Arc{par.get(), cpy.get()} {}
    void setAngles(std::uint16_t start, std::uint16_t end) { lv_arc_set_angles(obj, start, end); }
};

#endif