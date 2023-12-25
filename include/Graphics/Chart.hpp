#ifndef CHART_HPP
#define CHART_HPP

#include "display/lv_core/lv_obj.h"
#include "display/lv_misc/lv_area.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_objx/lv_chart.h"
#include "main.h"
#include "Graphics/LVGLObject.hpp"
#include <initializer_list>

class Chart : public LVGLObject{
public:
    Chart(lv_obj_t *par = lv_scr_act(), lv_obj_t *cpy = NULL): LVGLObject{lv_chart_create(par, cpy)} {}
    Chart(LVGLObject& par): Chart{par.get(), NULL} {}
    Chart(LVGLObject& par, Chart& cpy): Chart{par.get(), cpy.get()} {}
    lv_chart_series_t* operator[](const int i){ return series[i]; }
    void setType(lv_chart_type_t type) { lv_chart_set_type(obj, type); } 
    void setRange(lv_coord_t ymin, lv_coord_t ymax) { lv_chart_set_range(obj, ymin, ymax); }
    void setNextPoint(lv_coord_t y, int series_index) { lv_chart_set_next(obj, series[series_index], y); }
    void addSeries(lv_color_t color = LV_COLOR_MAKE(0, 0, 0)) { point_arr.emplace_back(); series.push_back(lv_chart_add_series(obj, color)); }
    void setPointCount(int count) { lv_chart_set_point_count(obj, count); }
    void setPoints(std::initializer_list<lv_coord_t> points, int series_index){
        point_arr[series_index].reset(new lv_coord_t[points.size()]);
        std::copy(points.begin(), points.end(), point_arr[series_index].get());
        lv_chart_set_point_count(obj, points.size());
        lv_chart_set_points(obj, series[series_index], point_arr[series_index].get());
    }
    void setPoints(std::vector<lv_coord_t> points, int series_index){
        point_arr[series_index].reset(new lv_coord_t[points.size()]);
        std::copy(points.begin(), points.end(), point_arr[series_index].get());
        lv_chart_set_point_count(obj, points.size());
        lv_chart_set_points(obj, series[series_index], point_arr[series_index].get());
    }
    lv_coord_t *getYVals(int series_index) { return point_arr[series_index].get();  }
    std::size_t getSeriesNum() { return series.size(); }
private:
    std::vector<lv_chart_series_t*> series;
    std::vector<std::shared_ptr<lv_coord_t> > point_arr;
};

#endif