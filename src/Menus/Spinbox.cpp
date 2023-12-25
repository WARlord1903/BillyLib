#include "main.h"
#include "Globals.hpp"
#include "Graphics/Menu.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Button.hpp"
#include <string>


struct SpinboxData {
    Menu* curr_menu;
    std::variant<int, double> init;
    unsigned num_digits;
    unsigned prec;
    int sign;
    std::variant<int, double> min;
    std::variant<int, double> max;
    std::string num_str;
    Image* digit_imgs;
    Image* sign_img;
};

std::string get_front_pad(double num, unsigned num_digits){
    std::string res = "";
    std::string num_str = std::to_string(num).substr(0, std::to_string(num).find('.'));
    while(res.size() < num_digits - num_str.size())
        res += '0';
    return res;
}

std::string get_front_pad(unsigned num, unsigned num_digits){
    std::string res = "";
    std::string num_str = std::to_string(num);
    while(res.size() < num_digits - num_str.size())
        res += '0';
    return res;
}

std::string get_back_pad(std::string num, unsigned num_digits){
    std::string res = num;
    while(res.size() < num_digits + 1)
        res += '0';
    return res;
}

std::string get_padded_num(double num, unsigned int num_digits, unsigned int prec){
    std::string res = get_front_pad(num, num_digits - prec);
    std::stringstream num_stream;
    num_stream << std::fixed << std::setprecision(prec) << std::abs(num);
    res += num_stream.str();
    if(res.find('.') == std::string::npos)
        res += '.';
    return get_back_pad(res, num_digits);
}

std::string get_padded_num(unsigned num, unsigned num_digits){
    return get_front_pad(num, num_digits) + std::to_string(num);
}

const std::array<const char*, 10> num_paths = {
    "S:/usd/numbers/0.bin",
    "S:/usd/numbers/1.bin",
    "S:/usd/numbers/2.bin",
    "S:/usd/numbers/3.bin",
    "S:/usd/numbers/4.bin",
    "S:/usd/numbers/5.bin",
    "S:/usd/numbers/6.bin",
    "S:/usd/numbers/7.bin",
    "S:/usd/numbers/8.bin",
    "S:/usd/numbers/9.bin",
};


void refresh_nums(SpinboxData* data){
    int char_counter = 0;
    for(auto& c : data->num_str){
        if(c != '.' && char_counter < data->num_digits + (data->prec != 0) ? 1 : 0)
            data->digit_imgs[char_counter++].setImagePath(num_paths[(int) (c - '0')]);
    }
    data->sign_img->setImagePath((data->sign == 1) ? "S:/usd/numbers/plus.bin" : "S:/usd/numbers/minus.bin");
}

lv_res_t save_num(lv_obj_t* btn){
    SpinboxData* data = (SpinboxData*) lv_obj_get_free_ptr(btn);
    data->curr_menu->getTask().notify();
    return LV_RES_INV;
}

void increase_num(SpinboxData* data, int index){
    std::size_t dot_pos = data->num_str.find('.');
    int pos;
    if(dot_pos > index || dot_pos == std::string::npos)
        pos = index;
    else
        pos = index + 1;
    if(data->num_str.at(pos) == '9')
        data->num_str.at(pos) = '0';
    else
        data->num_str.at(pos) += 1;
    if(data->prec == 0){
        if(std::stoi(data->num_str) * data->sign > std::get<int>(data->max)){
            data->num_str = get_padded_num(std::get<int>(data->max), data->num_digits);
            data->sign = sign(std::get<int>(data->max));
        }
        if(std::stoi(data->num_str) * data->sign < std::get<int>(data->min)){
            data->num_str = get_padded_num(std::get<int>(data->min), data->num_digits);
            data->sign = sign(std::get<int>(data->min));
        }
    }
    else{
        if(std::stod(data->num_str) * data->sign > std::get<double>(data->max)){
            data->num_str = get_padded_num(std::get<double>(data->max), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->max));
        }
        if(std::stod(data->num_str) * data->sign < std::get<double>(data->min)){
            data->num_str = get_padded_num(std::get<double>(data->min), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->min));
        }
    }
}

void decrease_num(SpinboxData* data, int index){
    std::size_t dot_pos = data->num_str.find('.');
    int pos;
    if(dot_pos > index || dot_pos == std::string::npos)
        pos = index;
    else
        pos = index + 1;
    if(data->num_str.at(pos) == '0')
        data->num_str.at(pos) = '9';
    else
        data->num_str.at(pos) -= 1;
    if(data->prec == 0){
        if(std::stoi(data->num_str) * data->sign > std::get<int>(data->max)){
            data->num_str = get_padded_num(std::get<int>(data->max), data->num_digits);
            data->sign = sign(std::get<int>(data->max));
        }
        if(std::stoi(data->num_str) * data->sign < std::get<int>(data->min)){
            data->num_str = get_padded_num(std::get<int>(data->min), data->num_digits);
            data->sign = sign(std::get<int>(data->min));
        }
    }
    else{
        if(std::stod(data->num_str) * data->sign > std::get<double>(data->max)){
            data->num_str = get_padded_num(std::get<double>(data->max), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->max));
        }
        if(std::stod(data->num_str) * data->sign < std::get<double>(data->min)){
            data->num_str = get_padded_num(std::get<double>(data->min), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->min));
        }
    }
}

void change_sign(SpinboxData* data){
    if(data->sign == 1)
        data->sign = -1;
    else
        data->sign = 1;
    if(data->prec == 0){
        if(std::stoi(data->num_str) * data->sign > std::get<int>(data->max)){
            data->num_str = get_padded_num(std::get<int>(data->max), data->num_digits);
            data->sign = sign(std::get<int>(data->max));
        }
        if(std::stoi(data->num_str) * data->sign < std::get<int>(data->min)){
            data->num_str = get_padded_num(std::get<int>(data->min), data->num_digits);
            data->sign = sign(std::get<int>(data->min));
        }
    }
    else{
        if(std::stod(data->num_str) * data->sign > std::get<double>(data->max)){
            data->num_str = get_padded_num(std::get<double>(data->max), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->max));
        }
        if(std::stod(data->num_str) * data->sign < std::get<double>(data->min)){
            data->num_str = get_padded_num(std::get<double>(data->min), data->num_digits, data->prec);
            data->sign = sign(std::get<double>(data->min));
        }
    }
}

lv_res_t increase_num_callback(lv_obj_t* btn){
    increase_num((SpinboxData*) lv_obj_get_free_ptr(btn), lv_obj_get_free_num(btn));
    return LV_RES_OK;
}

lv_res_t decrease_num_callback(lv_obj_t* btn){
    decrease_num((SpinboxData*) lv_obj_get_free_ptr(btn), lv_obj_get_free_num(btn));
    return LV_RES_OK;
}

lv_res_t change_sign_callback(lv_obj_t* btn){
    change_sign((SpinboxData*) lv_obj_get_free_ptr(btn));
    return LV_RES_OK;
}

void spinbox_exec(SpinboxData* data, std::string txt = ""){
    lv_obj_t* prev_screen = lv_scr_act();
    lv_obj_t* spinbox_scr = lv_obj_create(NULL, NULL);
    lv_scr_load(spinbox_scr);

    Label label(txt);
    label.align(LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_style_t select_style;
    lv_style_copy(&select_style, &lv_style_plain);
    select_style.image.color = LV_COLOR_RED;
    select_style.image.intense = LV_OPA_100;

    std::string num = "";
    if(std::holds_alternative<int>(data->init))
        num = get_padded_num(std::get<int>(data->init), data->num_digits);
    else
        num = get_padded_num(std::get<double>(data->init), data->num_digits, data->prec);

    Image num_arr[data->num_digits];
    for(int i = 0; i < data->num_digits; i++){
        num_arr[i].setSize(40, 80);
        num_arr[i].align(LV_ALIGN_IN_LEFT_MID, (40 * i) + ((i >= data->num_digits - data->prec) ? 80 : 40), 0, lv_scr_act());
    }

    Image sign_img;
    sign_img.setSize(40, 80);
    sign_img.align(LV_ALIGN_IN_LEFT_MID, 0, 0, lv_scr_act());
    sign_img.setImagePath((data->sign == 1) ? "S:/usd/numbers/plus.bin" : "S:/usd/numbers/minus.bin");

    data->digit_imgs = num_arr;
    data->sign_img = &sign_img;

    refresh_nums(data);
    
    Image dot;
    dot.setImagePath("S:/usd/numbers/dot.bin");
    dot.setSize(40, 80);
    dot.align(LV_ALIGN_IN_LEFT_MID, (data->num_digits - data->prec + 1) * 40, 0, lv_scr_act());
    
    if(data->prec == 0)
        dot.setHidden(true);

    Button increase_btns[data->num_digits];
    Button decrease_btns[data->num_digits];
    for(int i = 0; i < data->num_digits; i++){
        increase_btns[i].setText(SYMBOL_UP);
        decrease_btns[i].setText(SYMBOL_DOWN);
        increase_btns[i].setSize(40, 20);
        decrease_btns[i].setSize(40, 20);
        increase_btns[i].setFreeNum(i);
        decrease_btns[i].setFreeNum(i);
        increase_btns[i].setFreePtr(data);
        decrease_btns[i].setFreePtr(data);
        increase_btns[i].align(LV_ALIGN_OUT_TOP_MID, 0, 0, num_arr[i]);
        decrease_btns[i].align(LV_ALIGN_OUT_BOTTOM_MID, 0, 0, num_arr[i]);
        increase_btns[i].setAction(increase_num_callback);
        decrease_btns[i].setAction(decrease_num_callback);
    }

    Button increase_sign(SYMBOL_UP);
    increase_sign.setFreePtr(data);
    increase_sign.setSize(40, 20);
    increase_sign.align(LV_ALIGN_OUT_TOP_MID, 0, 0, sign_img);
    increase_sign.setAction(change_sign_callback);

    Button decrease_sign(SYMBOL_DOWN);
    decrease_sign.setFreePtr(data);
    decrease_sign.setSize(40, 20);
    decrease_sign.align(LV_ALIGN_OUT_BOTTOM_MID, 0, 0, sign_img);
    decrease_sign.setAction(change_sign_callback);
    
    Button save_btn("Save");
    save_btn.setFreePtr(data);
    save_btn.setSize(80, 40);
    save_btn.align(LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0, lv_scr_act());
    save_btn.setAction(save_num);
    
    int selected_digit = 0;
    bool dpad_select = false;

    while(1){
        if(pros::Task::notify_take(true, 0) || (dpad_select && c_w.toggled(ControllerDigital::A)))
            break;

        if(!dpad_select && (c_w.toggled(ControllerDigital::left) || c_w.toggled(ControllerDigital::right) || c_w.toggled(ControllerDigital::up) || c_w.toggled(ControllerDigital::down) || c_w.toggled(ControllerDigital::A))){
            dpad_select = true;
            sign_img.setStyle(&select_style);
        }
        if(dpad_select && c_w.isPressed(ControllerDigital::left)){
            if(selected_digit == -1)
                selected_digit = data->num_digits - 1;
            else
                --selected_digit;
            for(int i = 0; i < data->num_digits; i++)
                num_arr[i].setStyle(&lv_style_plain);
            sign_img.setStyle(&lv_style_plain);
            if(selected_digit == -1)
                sign_img.setStyle(&select_style);
            else
                num_arr[selected_digit].setStyle(&select_style);
            refresh_nums(data);
            pros::delay(480);
        }
        if(dpad_select && c_w.isPressed(ControllerDigital::right)){
            if(selected_digit == data->num_digits - 1)
                selected_digit = -1;
            else
                ++selected_digit;
            for(int i = 0; i < data->num_digits; i++)
                num_arr[i].setStyle(&lv_style_plain);
            sign_img.setStyle(&lv_style_plain);
            if(selected_digit == -1)
                sign_img.setStyle(&select_style);
            else
                num_arr[selected_digit].setStyle(&select_style);
            refresh_nums(data);
            pros::delay(480);
        }
        if(dpad_select && c_w.isPressed(ControllerDigital::up)){
            if(selected_digit != -1)
                increase_num(data, selected_digit);
            else
                change_sign(data);
            refresh_nums(data);
            pros::delay(480);
        }
        if(dpad_select && c_w.isPressed(ControllerDigital::down)){
            if(selected_digit != -1)
                decrease_num(data, selected_digit);
            else
                change_sign(data);
            refresh_nums(data);
            pros::delay(480);
        }
        pros::delay(20);
    }
    lv_scr_load(prev_screen);
    lv_obj_clean(spinbox_scr);
}

double spinbox(Menu& menu, double init, unsigned num_digits, unsigned prec, double min, double max, std::string txt){
    SpinboxData data = {&menu, std::abs(init), num_digits, prec, sign(init), min, max, get_padded_num(std::abs(init), num_digits, prec)};
    spinbox_exec(&data, txt);
    return std::stod(data.num_str);
}

int spinbox(Menu& menu, int init, unsigned num_digits, int min, int max, std::string txt){
    if(num_digits >= 9)
        num_digits = 8;
    SpinboxData data = {&menu, std::abs(init), num_digits, 0, sign(init), min, max, get_padded_num(std::abs(init), num_digits)};
    spinbox_exec(&data, txt);
    return std::stoi(data.num_str);
}