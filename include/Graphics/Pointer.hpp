#ifndef POINTER_HPP
#define POINTER_HPP

#include "main.h"
#include "Graphics/Line.hpp"

class Pointer{
public:
    Pointer(){
        hor.setPoints({{0, 0}, {6, 0}});
        ver.setPoints({{0, 0}, {0, 5}});
        hor.align(LV_ALIGN_CENTER, 0, 0);
        ver.align(LV_ALIGN_CENTER, 0, 0);
    }
private:
    Line hor;
    Line ver;
};

#endif