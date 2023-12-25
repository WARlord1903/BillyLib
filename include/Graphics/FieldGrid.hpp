#ifndef FIELDGRID_HPP
#define FIELDGRID_HPP

#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_btn.h"
#include "main.h"
#include "ImageButton.hpp"

class FieldGrid{
public:
    FieldGrid(){
        int tile_count = 0;

        for(int i = 0; i < tiles.size() / std::sqrt(tiles.size()); i++){
            for(int j = 0; j < tiles.size() / std::sqrt(tiles.size()); j++){
                if((i + j) % 2 == 0){
                    tiles[tile_count].setImagePath("S:/usd/Tile1.bin");
                    tiles[tile_count].setImagePath("S:/usd/Tile1_pr.bin", LV_BTN_STATE_PR);
                }
                else{
                    tiles[tile_count].setImagePath("S:/usd/Tile2.bin");
                    tiles[tile_count].setImagePath("S:/usd/Tile2_pr.bin", LV_BTN_STATE_PR);
                }
                tiles[tile_count++].setFreeNum(tile_count);
            }
        }

        for(int i = 0; i < tiles.size(); i++)
            tiles[i].align(LV_ALIGN_IN_BOTTOM_LEFT, LV_HOR_RES / 4 + (40 * (i % 6)), -(40 * (i / 6)));
    }
    void setFreePtr(void* ptr) { for(auto& t : tiles) t.setFreePtr(ptr); }
    void setAction(lv_action_t action, lv_btn_action_t type = LV_BTN_ACTION_CLICK) { for(auto& t : tiles) t.setAction(action, type); }
    void del() { for(auto& t : tiles) t.del(); }
private:
    std::array<ImageButton, 36> tiles;
};

#endif