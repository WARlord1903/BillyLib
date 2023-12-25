#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_page.h"
#include "display/lv_objx/lv_ta.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"

class TextArea : public LVGLObject{
public:
    TextArea(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_ta_create(par, cpy)} {}
    TextArea(LVGLObject& par): TextArea{par.get()} {}
    TextArea(LVGLObject& par, TextArea& cpy): TextArea{par.get(), cpy.get()} {}
    void addChar(std::uint32_t c) { lv_ta_add_char(obj, c); }
    void addText(const char* txt) { lv_ta_add_text(obj, txt); }
    void addText(std::string txt) { 
        curr_str = txt;
        lv_ta_add_text(obj, curr_str.c_str()); 
    }
    void delChar() { lv_ta_del_char(obj); }
    void setText(const char* txt) { lv_ta_set_text(obj, txt); }
    void setText(std::string txt) { 
        curr_str = txt;
        lv_ta_set_text(obj, curr_str.c_str()); 
    }
    void setPos(std::uint16_t pos) { lv_ta_set_cursor_pos(obj, pos); }
    void setCursorType(lv_cursor_type_t cur_type) { lv_ta_set_cursor_type(obj, cur_type); }
    void setPasswordMode(bool en) { lv_ta_set_pwd_mode(obj, en); }
    void setOneLine(bool en) { lv_ta_set_one_line(obj, en); }
    void setAlign(lv_label_align_t align) { lv_ta_set_text_align(obj, align); }
    void setAcceptedChars(const char* txt) { lv_ta_set_accepted_chars(obj, txt); }
    void setAcceptedChars(std::string txt) { 
        accepted_chars = txt;
        lv_ta_set_accepted_chars(obj, accepted_chars.c_str()); 
    }
    void setAction(lv_action_t action) { lv_ta_set_action(obj, action); }
    void setScrollMode(lv_sb_mode_t mode) { lv_ta_set_sb_mode(obj, mode); }
    void setScrollPropagation(bool en) { lv_ta_set_scroll_propagation(obj, en); }
    void setEdgeFlash(bool en) { lv_ta_set_edge_flash(obj, en); }
    std::string getText() { return lv_ta_get_text(obj); }
    std::uint16_t getPos() { return lv_ta_get_cursor_pos(obj); }
    lv_cursor_type_t getCursorType() { return lv_ta_get_cursor_type(obj); }
    bool getPasswordMode() { return lv_ta_get_pwd_mode(obj); }
    bool getOneLine() { return lv_ta_get_one_line(obj); }
    std::string getAcceptedChars() { return lv_ta_get_accepted_chars(obj); }
    lv_action_t getAction() { return lv_ta_get_action(obj); }
    lv_sb_mode_t getScrollMode() { return lv_ta_get_sb_mode(obj); }
    bool getScrollPropagation() { return lv_ta_get_scroll_propagation(obj); }
    bool getEdgeFlash() { return lv_ta_get_edge_flash(obj); }
    void up() { lv_ta_cursor_up(obj); }
    void down() { lv_ta_cursor_down(obj); }
    void left() { lv_ta_cursor_left(obj); }
    void right() { lv_ta_cursor_right(obj); }
private:
    std::string curr_str;
    std::string accepted_chars;
};

#endif