#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Graphics/LVGLObject.hpp"
#include "display/lv_objx/lv_img.h"
#include "display/lv_objx/lv_imgbtn.h"

class Image : public LVGLObject{
public:
    Image(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_img_create(par, cpy)} {}
    Image(LVGLObject& par): LVGLObject{lv_img_create(par.get(), NULL)} {}
    Image(LVGLObject& par, Image& cpy): LVGLObject{lv_img_create(par.get(), cpy.get())} {}
    void setImagePath(const void * src) { lv_img_set_src(obj, src); }
};

#endif