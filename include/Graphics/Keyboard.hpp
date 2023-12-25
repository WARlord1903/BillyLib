#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_kb.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"
#include "Graphics/TextArea.hpp"

class Keyboard : public LVGLObject{
public:
    Keyboard(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_kb_create(par, cpy)} {}
    Keyboard(LVGLObject& par): Keyboard{par.get()} {}
    Keyboard(LVGLObject& par, Keyboard& cpy): Keyboard{par.get(), cpy.get()} {}
    void setTextArea(lv_obj_t* ta) { lv_kb_set_ta(obj, ta); }
    void setTextArea(TextArea& ta) { lv_kb_set_ta(obj, ta.get()); }
    void setMode(lv_kb_mode_t mode) { lv_kb_set_mode(obj, mode); }
    void setAction(lv_action_t action) { lv_kb_set_ok_action(obj, action); }
    void setHideAction(lv_action_t action) { lv_kb_set_hide_action(obj, action); }
    void setMap(std::initializer_list<std::string> btns) { 
        std::vector<std::string> btn_labels{btns.begin(), btns.end()};
        if(btn_labels.back() == "")
            btn_labels.push_back("");
        btn_arr.reset(new const char * [btn_labels.size()]);
        std::transform(btn_labels.begin(), btn_labels.end(), btn_arr.get(), [](std::string& s) { return s.c_str(); });
        lv_btnm_set_map(obj, btn_arr.get());
     }

     lv_kb_mode_t getMode() { return lv_kb_get_mode(obj); }
     lv_action_t getAction() { return lv_kb_get_ok_action(obj); }
private:
    std::unique_ptr<const char*> btn_arr;
};

#endif