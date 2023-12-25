#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Graphics/LVGLObject.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_misc/lv_area.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_objx/lv_btn.h"
#include "main.h"
#include "Graphics/Label.hpp"
#include <string>

class Button : public LVGLObject{
public:
    Button(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject(lv_btn_create(par, cpy)), label{" "} { label.align(LV_ALIGN_CENTER, 0, 0, obj); }
    Button(Button& par): Button(par.get()) {}
    Button(Button& par, Button& cpy): Button(par.get(), cpy.get()) {}
    Button(std::string text, lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL, lv_res_t (*action)(lv_obj_t*) = NULL, lv_btn_action_t type = LV_BTN_ACTION_CLICK): Button(par, cpy) { label.setText(text); setAction(action, type); }
    Button(std::string text, Button& par, lv_res_t (*action)(lv_obj_t*) = NULL, lv_btn_action_t type = LV_BTN_ACTION_CLICK): Button(par) { label.setText(text); setAction(action, type); }
    Button(std::string text, Button& par, Button& cpy): Button(par, cpy) { label.setText(text); }
    void del() override { LVGLObject::del(); label.del(); }
    void setAction(lv_res_t (*action)(lv_obj_t*), lv_btn_action_t type = LV_BTN_ACTION_CLICK){ lv_btn_set_action(obj, type, action); }
    void setText(const char* text) { label.setText(text); label.realign(); }
    void setText(std::string text) { label.setText(text); label.realign(); }
    void setHidden(bool en) override { lv_obj_set_hidden(obj, en); label.setHidden(en); }
    void setSize(lv_coord_t w, lv_coord_t h) override { lv_obj_set_size(obj, w, h); label.realign(); }
    void setPos(lv_coord_t x, lv_coord_t y) override { lv_obj_set_pos(obj, x, y); label.realign(); }
    void setX(lv_coord_t x) override { lv_obj_set_x(obj, x); label.realign(); }
    void setY(lv_coord_t y) override { lv_obj_set_y(obj, y); label.realign(); }
    void setWidth(lv_coord_t w) override { lv_obj_set_width(obj, w); label.realign(); }
    void setHeight(lv_coord_t h) override { lv_obj_set_height(obj, h); label.realign(); }
    void realign() override { lv_obj_realign(obj); label.realign(); }
    void setAutoRealign(bool en) override { lv_obj_set_auto_realign(obj, en); label.setAutoRealign(en); }
    void align(lv_align_t align, lv_coord_t x, lv_coord_t y, lv_obj_t* base = lv_scr_act()) override { LVGLObject::align(align, x, y, base); label.realign(); }
    void align(lv_align_t align, lv_coord_t x, lv_coord_t y, LVGLObject& base) override { LVGLObject::align(align, x, y, base); label.realign(); }
private:
    Label label;
};

#endif