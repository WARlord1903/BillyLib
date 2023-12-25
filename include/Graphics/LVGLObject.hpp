#ifndef LVGLOBJECT_HPP
#define LVGLOBJECT_HPP

#include "main.h"
#include <type_traits>
#include <utility>

class LVGLObject{
public:
    lv_obj_t *get() { return obj; }
    virtual void setSize(lv_coord_t w, lv_coord_t h) { lv_obj_set_size(obj, w, h); }
    virtual void setPos(lv_coord_t x, lv_coord_t y) { lv_obj_set_pos(obj, x, y); }
    virtual void setX(lv_coord_t x) { lv_obj_set_x(obj, x); }
    virtual void setY(lv_coord_t y) { lv_obj_set_y(obj, y); }
    virtual void setWidth(lv_coord_t w) { lv_obj_set_width(obj, w); }
    virtual void setHeight(lv_coord_t h) { lv_obj_set_height(obj, h); }
    virtual void align(lv_align_t align, lv_coord_t x, lv_coord_t y, lv_obj_t *base = lv_scr_act()) { lv_obj_align(obj, base, align, x, y); }
    virtual void align(lv_align_t align, lv_coord_t x, lv_coord_t y, LVGLObject &base) { lv_obj_align(obj, base.obj, align, x, y); }
    virtual void realign() { lv_obj_realign(obj); }
    virtual void setAutoRealign(bool en) { lv_obj_set_auto_realign(obj, en); }
    void setStyle(lv_style_t *style) { lv_obj_set_style(obj, style); }
    void refreshStyle() { lv_obj_refresh_style(obj); }
    virtual void setHidden(bool en) { lv_obj_set_hidden(obj, en); }
    void setClickable(bool en) { lv_obj_set_click(obj, en); }
    void setTop(bool en) { lv_obj_set_top(obj, en); }
    void setDrag(bool en) { lv_obj_set_drag(obj, en); }
    void setThrow(bool en) { lv_obj_set_drag_throw(obj, en); }
    void setDragParent(bool en) { lv_obj_set_drag_parent(obj, en); }
    void setOPAScale(bool en) { lv_obj_set_opa_scale_enable(obj, en); }
    void setOPA(lv_opa_t opa_scale) { lv_obj_set_opa_scale(obj, opa_scale); }
    void setFreeNum(LV_OBJ_FREE_NUM_TYPE num) { lv_obj_set_free_num(obj, num); }
    void setFreePtr(void *free_p) { lv_obj_set_free_ptr(obj, free_p); }
    void setParent(lv_obj_t *parent) { lv_obj_set_parent(obj, parent); }
    void setParent(LVGLObject &parent) { lv_obj_set_parent(obj, parent.obj); }
    lv_obj_t *getChild(lv_obj_t *child) { return lv_obj_get_child(obj, child); }
    lv_obj_t *getChild(LVGLObject &child) { return lv_obj_get_child(obj, child.obj); }
    lv_obj_t *getChildBack(lv_obj_t *child) { return lv_obj_get_child_back(obj, child); }
    lv_obj_t *getChildBack(LVGLObject &child) { return lv_obj_get_child_back(obj, child.obj); }
    std::uint16_t getChildCount() { return lv_obj_count_children(obj); }
    lv_coord_t getX() { return lv_obj_get_x(obj); }
    lv_coord_t getY() { return lv_obj_get_y(obj); }
    lv_coord_t getWidth() { return lv_obj_get_width(obj); }
    lv_coord_t getHeight() { return lv_obj_get_height(obj); }
    bool getAutoRealign() { return lv_obj_get_auto_realign(obj); }
    lv_style_t *getStyle() { return lv_obj_get_style(obj); }
    bool getHidden() { return lv_obj_get_hidden(obj); }
    bool getClickable() { return lv_obj_get_click(obj); }
    bool getTop() { return lv_obj_get_top(obj); }
    bool getDrag() { return lv_obj_get_drag(obj); }
    bool getThrow() { return lv_obj_get_drag_throw(obj); }
    bool getDragParent() { return lv_obj_get_drag_parent(obj); }
    lv_opa_t getOPAScale() { return lv_obj_get_opa_scale_enable(obj); }
    lv_opa_t getOPA() { return lv_obj_get_opa_scale(obj); }
    LV_OBJ_FREE_NUM_TYPE getFreeNum() { return lv_obj_get_free_num(obj); }
    void *getFreePtr() { return lv_obj_get_free_ptr(obj); }
    lv_obj_t *getParent() { return lv_obj_get_parent(obj); }
    lv_obj_t *getScreen() { return lv_obj_get_screen(obj); }
    virtual void del() { if(obj) lv_obj_del(obj); }
    void clean() { lv_obj_clean(obj); }
protected:
    LVGLObject(lv_obj_t *obj_ptr, lv_style_t *style_ptr = NULL): obj{obj_ptr} {}
    LVGLObject(const LVGLObject& o) { obj = o.obj; }
    LVGLObject(LVGLObject&& o) { obj = std::move(o.obj); }
    lv_obj_t* obj;
};

#endif