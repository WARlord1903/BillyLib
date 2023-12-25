#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "main.h"
#include "Graphics/Menu.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/ButtonMatrix.hpp"
#include <initializer_list>

struct PageData {
    Menu& m;
    std::vector<std::string> vec;
    ButtonMatrix& btnm;
    bool exit;
    std::string* str_ptr;
    Button& left_arr;
    Button& right_arr;
};

lv_res_t return_selection(lv_obj_t *btn, const char* txt){
    PageData* data = (PageData*) lv_obj_get_free_ptr(btn);
    *(data->str_ptr) = txt;
    data->m.getTask().notify();
    return LV_RES_INV;
}

lv_res_t change_page(lv_obj_t*);

void button_matrix_page_exec(PageData& data, unsigned page_num){
    if(data.vec.back() == "\n")
        data.vec.back() = "";

    std::vector<unsigned> newline_indices;
    for(int i = 0; i < data.vec.size(); i++){
        if(data.vec[i] == "\n")
            newline_indices.push_back(i);
    }

    std::vector<std::string> btns;

    if(page_num * 3 + 2 >= newline_indices.size()){
        if(page_num == 0)
            for(int i = 0; i < data.vec.size(); i++)
                btns.push_back(data.vec[i]);
        else
            for(int i = newline_indices[page_num * 3 - 1] + 1; i < data.vec.size(); i++)
                btns.push_back(data.vec[i]);
    }
    else{
        if(page_num == 0)
            for(int i = 0; i < newline_indices[2]; i++)
                btns.push_back(data.vec[i]);
        else
            for(int i = newline_indices[page_num * 3 - 1] + 1; i < newline_indices[page_num * 3 + 2]; i++)
                btns.push_back(data.vec[i]);
    }


    data.left_arr.setHidden(false);
    data.left_arr.setFreePtr(&data);
    data.left_arr.setFreeNum(page_num - 1);
    if(page_num == 0)
        data.left_arr.setHidden(true);
    

    data.right_arr.setHidden(false);
    data.right_arr.setFreePtr(&data);
    data.right_arr.setFreeNum(page_num + 1);
    if(btns.back() == ""){
        data.right_arr.setHidden(true);
        btns.pop_back();
    }

    if(btns.back() == "\n")
        btns.pop_back();


    if(data.exit){
        btns.push_back("\n");
        btns.push_back("Exit");
        btns.push_back("");
    }

    data.btnm.setMap(btns);
    data.btnm.setSize(LV_HOR_RES - 80, LV_VER_RES);
    data.btnm.setFreePtr(&data);
    data.btnm.align(LV_ALIGN_CENTER, 0, 0);
    data.btnm.setAction(return_selection);
}

lv_res_t change_page(lv_obj_t* btn){
    PageData* data = (PageData*) lv_obj_get_free_ptr(btn);
    button_matrix_page_exec(*data, lv_obj_get_free_num(btn));
    return LV_RES_INV;
}

std::string button_matrix_page(Menu& menu, std::vector<std::string>& vec, bool exit, unsigned page_num){

    lv_obj_t* prev_scr = lv_scr_act();
    lv_obj_t* btnm_scr = lv_obj_create(NULL, NULL);
    lv_scr_load(btnm_scr);

    Button left_arr(SYMBOL_LEFT, lv_scr_act(), NULL, change_page);
    left_arr.setSize(40, 40);
    left_arr.align(LV_ALIGN_IN_LEFT_MID, 0, 0);
    left_arr.setFreeNum(page_num - 1);

    Button right_arr(SYMBOL_RIGHT, lv_scr_act(), NULL, change_page);
    right_arr.setSize(40, 40);
    right_arr.align(LV_ALIGN_IN_RIGHT_MID, 0, 0);
    right_arr.setFreeNum(page_num + 1);
    ButtonMatrix btnm;

    std::string ret;

    PageData data = {menu, vec, btnm, exit, &ret, left_arr, right_arr};

    button_matrix_page_exec(data, page_num);

    wait_until_notified(menu);

    lv_scr_load(prev_scr);
    lv_obj_del(btnm_scr);
    
    return ret;
}

std::string button_matrix_page(Menu& menu, std::initializer_list<std::string> list, bool exit, unsigned page_num){
    std::vector<std::string> vec(list.begin(), list.end());
    std::string ret = button_matrix_page(menu, vec, exit, page_num);
    return ret;
}