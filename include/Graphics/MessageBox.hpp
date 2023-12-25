#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include "display/lv_objx/lv_btnm.h"
#include "display/lv_objx/lv_mbox.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"
#include <string>

class MessageBox : public LVGLObject{
public:
    MessageBox(lv_obj_t * par = lv_scr_act(), lv_obj_t * cpy = NULL): LVGLObject(lv_mbox_create(par, cpy)) {}
    MessageBox(std::string itext, std::initializer_list<std::string> btns, lv_obj_t * par = lv_scr_act(), lv_obj_t * cpy = NULL, lv_btnm_action_t action = NULL): MessageBox{par, cpy} { 
        setText(itext);
        setMap(btns);
        setAction(action);
    }
    MessageBox(std::string itext, std::initializer_list<std::string> btns, LVGLObject& par, lv_btnm_action_t action = NULL): MessageBox{itext, btns, par.get(), NULL} { }
    MessageBox(std::string itext, std::initializer_list<std::string> btns, LVGLObject& par, MessageBox& cpy, lv_btnm_action_t action = NULL): MessageBox{itext, btns, par.get(), cpy.get()} {}
   
    MessageBox(std::string itext, std::initializer_list<std::string> btns, lv_coord_t w, lv_coord_t h, lv_obj_t * par = lv_scr_act(), lv_obj_t * cpy = NULL, lv_btnm_action_t action = NULL): MessageBox{itext, btns, par, cpy, action} { 
        setSize(w, h);
    }
    MessageBox(std::string itext, std::initializer_list<std::string> btns, lv_coord_t w, lv_coord_t h, LVGLObject& par, lv_btnm_action_t action = NULL): MessageBox{itext, btns, w, h, par.get(), NULL, action} {}
    MessageBox(std::string itext, std::initializer_list<std::string> btns, lv_coord_t w, lv_coord_t h, LVGLObject& par, MessageBox& cpy, lv_btnm_action_t action = NULL): MessageBox{itext, btns, w, h, par.get(), cpy.get(), action} {}

    void setMap(std::initializer_list<std::string> btns){
        std::vector<std::string> btn_labels{btns.begin(), btns.end()};
        btn_labels.push_back("");
        btn_arr.reset(new const char * [btn_labels.size()]);
        std::transform(btn_labels.begin(), btn_labels.end(), btn_arr.get(), [](std::string& s) { return s.c_str(); });
        lv_btnm_set_map(obj, btn_arr.get());
    }

    void setText(std::string itext) { text = itext; lv_mbox_set_text(obj, text.c_str()); }
    void setAction(lv_btnm_action_t action) { lv_btnm_set_action(obj, action); }

private:
    std::string text;
    std::unique_ptr<const char*> btn_arr;
};

#endif