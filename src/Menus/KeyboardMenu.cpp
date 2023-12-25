#include "Graphics/Menu.hpp"
#include "display/lv_conf.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_kb.h"
#include "display/lv_objx/lv_label.h"
#include "main.h"
#include "Graphics/Keyboard.hpp"
#include "Graphics/TextArea.hpp"
#include <string>

struct MenuStr{
    Menu& m;
    std::string* str_ptr;
};

lv_res_t hide_action(lv_obj_t *kb){
    return LV_RES_INV;
}

lv_res_t submit_str(lv_obj_t *kb){
    MenuStr* data = (MenuStr*) lv_obj_get_free_ptr(kb);
    *(data->str_ptr) = lv_ta_get_text(lv_kb_get_ta(kb));
    data->m.getTask().notify();
    return LV_RES_INV;
}

std::string keyboard_menu(Menu& menu, bool nums){
    lv_obj_t* prev_scr = lv_scr_act();
    lv_obj_t* keyboard_scr = lv_obj_create(NULL, NULL);
    lv_scr_load(keyboard_scr);

    std::string ret;

    Keyboard kb;
    TextArea ta;

    MenuStr data = {menu, &ret};

    ta.setSize(LV_HOR_RES, LV_VER_RES - (LV_VER_RES - 60));
    kb.setSize(LV_HOR_RES - 60, LV_VER_RES - 60);

    ta.align(LV_ALIGN_IN_TOP_MID, 0, 0);
    kb.setMode(LV_KB_MODE_TEXT);
    kb.align(LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    ta.setOneLine(true);
    ta.setText("");
    ta.setAlign(LV_LABEL_ALIGN_CENTER);

    kb.setFreePtr(&data);
    kb.setAction(submit_str);
    kb.setHideAction(hide_action);
    kb.setTextArea(ta);

    wait_until_notified(menu);

    lv_scr_load(prev_scr);
    lv_obj_del(keyboard_scr);

    return ret;
}