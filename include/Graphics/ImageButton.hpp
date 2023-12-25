#ifndef IMAGEBUTTON_HPP
#define IMAGEBUTTON_HPP

#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_imgbtn.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"

class ImageButton : public LVGLObject{
public:
    ImageButton(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_imgbtn_create(par, cpy)} {}
    ImageButton(LVGLObject& par): LVGLObject{lv_imgbtn_create(par.get(), NULL)} {}
    ImageButton(LVGLObject& par, ImageButton& cpy): LVGLObject{lv_imgbtn_create(par.get(), cpy.get())} {}
    void setImagePath(const void* src, lv_btn_state_t state = LV_BTN_STATE_REL) { lv_imgbtn_set_src(obj, state, src); }
    void setAction(lv_action_t action, lv_btn_action_t type = LV_BTN_ACTION_CLICK) { lv_imgbtn_set_action(obj, type, action); }
};

#endif