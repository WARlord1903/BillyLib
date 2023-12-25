#ifndef STYLE_HPP
#define STYLE_HPP

#include "display/lv_core/lv_style.h"
#include "display/lv_misc/lv_area.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_misc/lv_font.h"
#include "main.h"
class Style{
public:
    Style(lv_style_t* s = nullptr): style{(s) ? s : new lv_style_t(lv_style_pretty_color)} {}
    inline lv_style_t* get() { return style.get(); }
    inline void copy(lv_style_t* s) { lv_style_copy(style.get(), s); }
    inline void copy(Style& s) { lv_style_copy(style.get(), s.style.get()); }

    inline void setMainColor(lv_color_t color) { style->body.main_color = color; }
    inline void setGradColor(lv_color_t color) { style->body.grad_color = color; }
    inline void setRadius(lv_coord_t rad) { style->body.radius = rad; }
    inline void setOPA(lv_opa_t opa) { style->body.opa = opa; }

    inline void setBorderColor(lv_color_t color) { style->body.border.color = color; }
    inline void setBorderWidth(lv_coord_t width) { style->body.border.width = width; }
    inline void setBorderPart(lv_border_part_t part) { style->body.border.part = part; }
    inline void setBorderOPA(lv_opa_t opa) { style->body.border.opa = opa; }

    inline void setShadowColor(lv_color_t color) { style->body.shadow.color = color; }
    inline void setShadowWidth(lv_coord_t width) { style->body.shadow.width = width; }
    inline void setShadowType(lv_shadow_type_t type) { style->body.shadow.type = type; }

    inline void setHorPadding(lv_coord_t hor) { style->body.padding.hor = hor; }
    inline void setVerPadding(lv_coord_t ver) { style->body.padding.hor = ver; }
    inline void setInnerPadding(lv_coord_t inner) { style->body.padding.hor = inner; }

    inline void setTextColor(lv_color_t color) { style->text.color = color; }
    inline void setTextFont(lv_font_t* font) { style->text.font = font; }
    inline void setTextLetterSpacing(lv_coord_t spacing) { style->text.letter_space = spacing; }
    inline void setTextLineSpacing(lv_coord_t spacing) { style->text.line_space = spacing; }
    inline void setTextOPA(lv_opa_t opa) { style->text.opa = opa; }

    inline void setImageColor(lv_color_t color) { style->image.color = color; }
    inline void setImageIntensity(lv_opa_t intense) { style->image.intense = intense; }
    inline void setImageOPA(lv_opa_t opa) { style->image.opa = opa; }

    inline void setLineColor(lv_color_t color) { style->line.color = color; }
    inline void setLineWidth(lv_coord_t width) { style->line.width = width; }
    inline void setLineOPA(lv_opa_t opa) { style->line.opa = opa; }
private:
    std::unique_ptr<lv_style_t> style;
};

#endif