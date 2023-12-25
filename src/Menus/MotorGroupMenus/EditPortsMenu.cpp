#include "display/lv_core/lv_obj.h"
#include "main.h"
#include "Globals.hpp"
#include "Graphics/ButtonMatrix.hpp"
#include "Graphics/TextArea.hpp"
#include "Graphics/Menu.hpp"
#include <string>

struct PortMenu{
    std::list<std::int8_t>& ports;
    TextArea& port_text;
    Menu& m;
};

lv_res_t add_ports(lv_obj_t* btn, const char* txt){
    PortMenu* data = (PortMenu*) lv_obj_get_free_ptr(btn);
    if(strcmp(txt, "Confirm") == 0 && !data->ports.empty())
        data->m.getTask().notify();
    else if(strcmp(txt, "Reverse") == 0){
        if(!data->ports.empty()){
            data->ports.back() = -data->ports.back();
            data->ports.sort( [](std::int8_t a, std::int8_t b) { return std::abs(a) < std::abs(b); } );
            std::string text;
            for(auto& port : data->ports)
                text += std::to_string(port) + ", ";
            text = text.substr(0, text.size() - 2);
            data->port_text.setText(text);
        }
    }
    else if(strcmp(txt, "Delete") == 0){
        if(!data->ports.empty()){
            std::string text = "";
            data->ports.pop_back();
            for(auto& p : data->ports)
                text += std::to_string(p) + ", ";
            text = text.substr(0, text.size() - 2);
            data->port_text.setText(text);
        }
    }
    else{
        if(std::find(data->ports.begin(), data->ports.end(), std::atoi(txt)) == data->ports.end()){
            data->ports.push_back(std::atoi(txt));
            data->ports.sort( [](std::int8_t a, std::int8_t b) { return std::abs(a) < std::abs(b); } );
            std::string text;
            for(auto& port : data->ports)
                text += std::to_string(port) + ", ";
            text = text.substr(0, text.size() - 2);
            data->port_text.setText(text);
        }
    }
    return LV_RES_OK;
}

void editPortsMenu(Menu& menu, void* group_name_ptr){
    std::string group_name = *(std::string*) group_name_ptr;
    std::string text = "";
    std::list<std::int8_t> ports;

    if(motor_map.find(group_name) != motor_map.end()){
        ports = motor_map[group_name].getPorts();
        ports.sort( [](std::int8_t a, std::int8_t b) { return std::abs(a) < std::abs(b); } );
        for(auto& port : ports)
            text += std::to_string(port) + ", ";
        text = text.substr(0, text.size() - 2); 
    }

    TextArea port_text;
    port_text.setText(text);
    port_text.setAlign(LV_LABEL_ALIGN_CENTER);
    port_text.setSize(LV_HOR_RES, 60);
    port_text.align(LV_ALIGN_IN_TOP_MID, 0, 0);
    
    PortMenu data = {ports, port_text, menu};

    ButtonMatrix port_btnm;
    port_btnm.setMap(
                {"1", "2", "3", "4", "5", "\n",
                       "6", "7", "8", "9", "10", "\n",
                       "11", "12", "13", "14", "15", "\n",
                       "16", "17", "18", "19", "20", "\n",
                       "21", "Reverse", "Confirm", "Delete", ""
                      }
                    );
    port_btnm.setSize(LV_HOR_RES - 40, LV_VER_RES - 60);
    port_btnm.align(LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    port_btnm.setFreePtr(&data);
    port_btnm.setAction(add_ports);

    wait_until_notified(menu);

    motor_map[group_name] = ports;

    write_motors(ini);

    menu.setCallback(editMotorGroupMenu, &group_name);
}