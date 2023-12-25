#ifndef LINE_HPP
#define LINE_HPP

#include "display/lv_misc/lv_area.h"
#include "display/lv_objx/lv_line.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"
#include "MathUtils/PurePursuitPath.hpp"
#include <algorithm>
#include <initializer_list>
#include <vector>

class Line : public LVGLObject{
public:
    Line(lv_obj_t* par = lv_scr_act(), lv_obj_t* cpy = NULL): LVGLObject{lv_line_create(par, cpy)} {}
    Line(LVGLObject& par): Line(par.get(), NULL) {}
    Line(LVGLObject& par, Line& cpy): Line{par.get(), cpy.get()} {}
    Line(const Line& l): LVGLObject{l} { points = l.points; }
    Line(Line&& l): LVGLObject{l} { points = std::move(l.points); }
    
    void setYInvert(bool inv) { lv_line_set_y_invert(obj, inv); }

    unsigned getSize() { return size; }

    void setPoints(std::vector<lv_point_t> ipoints){
        size = ipoints.size();
        points.reset(new lv_point_t [size]);
        std::copy(ipoints.begin(), ipoints.end(), points.get());
        lv_line_set_points(obj, points.get(), size);
    }

    void setPoints(std::initializer_list<lv_point_t> ipoints) { 
        std::vector<lv_point_t> p{ipoints.begin(), ipoints.end()};
        setPoints(p);
    }

    void setPoints(Curve p){
        size = p.size();
        points.reset(new lv_point_t [size]);
        std::transform(p.begin(), p.end(), points.get(), [](Point2D& point) { return inchesToPixels(point.x, 144 - point.y); });
        lv_line_set_points(obj, points.get(), size);
    }

    void addPoint(lv_point_t p) {
        std::vector<lv_point_t> temp;
        for(int i = 0; i < size; i++)
            temp.push_back(points.get()[i]);
        temp.push_back(p);
        size = temp.size();
        points.reset(new lv_point_t[size]);
        std::copy(temp.begin(), temp.end(), points.get());
        lv_line_set_points(obj, points.get(), size);
    }

    std::vector<lv_point_t> getPoints(){
        std::vector<lv_point_t> ret;
        for(int i = 0; i < size; i++)
            ret.push_back(points.get()[i]);
        return ret;
    }
    std::shared_ptr<lv_point_t> points;
    unsigned size;
};

#endif