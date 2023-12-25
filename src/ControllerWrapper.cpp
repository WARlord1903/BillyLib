#include "ControllerWrapper.hpp"

void read(void* data_ptr){
    ControllerWrapper::ExControllerData* data = (ControllerWrapper::ExControllerData*) data_ptr;
    while(1){
        data->process_command();
        pros::delay(20);
    }
}

void ControllerWrapper::ExControllerData::process_command(){
    std::string line = datastream->getline();
    max = (line[0] << 8) + line[1];
    left_x = (((line[2] << 8) + line[3]) - max / 2.) / (max / 2.);
    left_y = (max - ((line[4] << 8) + line[5]) - max / 2.) / (max / 2.);
    right_x = (((line[6] << 8) + line[7]) - max / 2.) / (max / 2.);
    right_y = (max - ((line[8] << 8) + line[9]) - max / 2.) / (max / 2.);
    std::uint16_t buttons = (line[10] << 8) + line[11];
    for(int i = 0; i < pressed.size(); i++)
        pressed[i] = buttons & (0x1 << i);
}