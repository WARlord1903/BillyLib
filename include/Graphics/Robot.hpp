#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "Graphics/LVGLObject.hpp"
#include "MotionControllers/PurePursuitController.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "main.h"
#include "Graphics/Image.hpp"
#include "Graphics/Arc.hpp"
#include "Graphics/Line.hpp"
#include "OdomChassis.hpp"
#include "pros/rtos.hpp"
#include <memory>

void updateCoords(void *robot);

class Robot{
public:
    Robot(){
        robot_img.setImagePath("S:/usd/Robot.bin"); 
        lookahead.setAngles(0, 360); 
        lv_style_copy(&lookahead_style, &lv_style_pretty);
        lookahead_style.line.width = 5;
        lookahead.setStyle(&lookahead_style);
        lookahead.align(LV_ALIGN_CENTER, 0, 0, robot_img); 
        lookahead.setHidden(true); 
    }
    Robot(lv_obj_t* par): robot_img{par} {
        robot_img.setImagePath("S:/usd/Robot.bin"); 
        lookahead.setAngles(0, 360);  
        lv_style_copy(&lookahead_style, &lv_style_pretty);
        lookahead_style.line.width = 5;
        lookahead.setStyle(&lookahead_style);
        lookahead.align(LV_ALIGN_CENTER, 0, 0, robot_img); 
        lookahead.setHidden(true); 
    }
    Robot(LVGLObject& par): robot_img{par} {
        robot_img.setImagePath("S:/usd/Robot.bin"); 
        lookahead.setAngles(0, 360);  
        lv_style_copy(&lookahead_style, &lv_style_pretty);
        lookahead_style.line.width = 5;
        lookahead.setStyle(&lookahead_style);
        lookahead.align(LV_ALIGN_CENTER, 0, 0, robot_img); 
        lookahead.setHidden(true); 
    }
    Image& get() { return robot_img; }
    void setPos(lv_coord_t x, lv_coord_t y) { robot_img.setPos(x, y); lookahead.realign(); }
    void hide(bool en) { robot_img.setHidden(en); if(en) heading.setHidden(en); if(en) lookahead.setHidden(en); }
    void updateLookahead(double l) { lookahead.setHidden(false); lv_coord_t size = inchesToPixels(l * 2); lookahead.setSize(size, size); lookahead.realign(); }
    bool isLookaheadHidden() { return lookahead.getHidden(); }
    void hideHeading(bool en) { heading.setHidden(en); }
    void setAngle(double rad) { heading.setPoints({{(lv_coord_t)(robot_img.getX() + 10), (lv_coord_t)(robot_img.getY() + 10)}, {(lv_coord_t)(robot_img.getX() + 10 + 20 * std::cos(rad)), (lv_coord_t)(robot_img.getY() + 10 + 20 * std::sin(M_PI * 2 - rad))}}); }
    void align(lv_align_t al, lv_coord_t x, lv_coord_t y, lv_obj_t *base = lv_scr_act()) { robot_img.align(al, x, y, base); }
    void align(lv_align_t al, lv_coord_t x, lv_coord_t y, LVGLObject& base) { align(al, x, y, base.get()); }
    void setHidden(bool en) { robot_img.setHidden(en); heading.setHidden(en); if(!lookahead.getHidden()) lookahead.setHidden(en); }
    void hideLookahead(bool en) { lookahead.setHidden(en); }
private:
    Image robot_img;
    Line heading;
    Arc lookahead;
    lv_style_t lookahead_style;
};

#endif