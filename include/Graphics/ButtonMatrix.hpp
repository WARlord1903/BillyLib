#ifndef BUTTONMATRIX_HPP
#define BUTTONMATRIX_HPP

#include "main.h"
#include "Graphics/LVGLObject.hpp"

class ButtonMatrix : public LVGLObject{
public:
    ButtonMatrix(lv_obj_t * par = lv_scr_act(), lv_obj_t * cpy = NULL): LVGLObject(lv_btnm_create(par, cpy)) {}
    ButtonMatrix(LVGLObject& par): ButtonMatrix{par.get()} {}
    ButtonMatrix(LVGLObject& par, ButtonMatrix& cpy): ButtonMatrix{par.get(), cpy.get()} {} 

    void setAction(lv_btnm_action_t action) { lv_btnm_set_action(obj, action); }

    void setMap(std::initializer_list<std::string> btns){
        btn_labels.clear();
        for(auto it = btns.begin(); it != btns.end(); ++it)
            btn_labels.push_back(*it);
        if(btn_labels.back() != "")
            btn_labels.push_back("");
        btn_arr.reset(new const char * [btn_labels.size()]);
        std::transform(btn_labels.begin(), btn_labels.end(), btn_arr.get(), [](std::string& s) { return s.c_str(); });
        lv_btnm_set_map(obj, btn_arr.get());
    }

    void setMap(std::vector<std::string>& btns){
        btn_labels = btns;
        if(btn_labels.back() != "")
            btn_labels.push_back("");
        btn_arr.reset(new const char * [btn_labels.size()]);
        std::transform(btn_labels.begin(), btn_labels.end(), btn_arr.get(), [](std::string& s) { return s.c_str(); });
        lv_btnm_set_map(obj, btn_arr.get());
    }

    void setMap(const char** btns) { lv_btnm_set_map(obj, btns); }
    const char **getMap() { return lv_btnm_get_map(obj); }

private:
    std::unique_ptr<const char*> btn_arr;
    std::vector<std::string> btn_labels;
};

#endif