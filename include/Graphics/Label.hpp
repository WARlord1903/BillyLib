#ifndef LABEL_HPP
#define LABEL_HPP

#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"

class Label : public LVGLObject{
public:
    Label(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject{lv_label_create(par, cpy)} {}
    Label(std::string text, lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): Label{par, cpy} { setText(text); }
    Label(std::string text, LVGLObject& par): Label{text, par.get()} {}
    Label(std::string text, LVGLObject& par, LVGLObject& cpy): Label{text, par.get(), cpy.get()} {}
    void setText(const char* text) { lv_label_set_text(obj, text); }
    void setText(std::string text) { lv_label_set_text(obj, text.c_str()); }
    void setLongMode(lv_label_long_mode_t long_mode) { lv_label_set_long_mode(obj, long_mode); }
    void setAlign(lv_label_align_t align) { lv_label_set_align(obj, align); }
    std::string getText() const { return std::string(lv_label_get_text(obj)); }
    lv_label_long_mode_t getLongMode() const { return lv_label_get_long_mode(obj); }
    lv_label_align_t getAlign() const { return lv_label_get_align(obj); }
    void insert(std::string text, std::uint32_t pos = LV_LABEL_POS_LAST) { lv_label_ins_text(obj, pos, text.c_str()); }
    void cut(std::uint32_t pos = 0, std::uint32_t cnt = 0) { lv_label_cut_text(obj, pos, cnt); }
};

#endif