#include "Graphics/Menu.hpp"
#include "Globals.hpp"

void buttonActionMenu(Menu &m, void* btn_ptr){
    ControllerDigital btn;
    if(btn_ptr)
        btn = *(ControllerDigital*) btn_ptr;
    else{
        std::string selection = button_matrix_page(m, {"L1", "L2", "R1", "R2", "\n",
                                                            "A", "B", "X", "Y", "\n",
                                                            "Up", "Down", "Left", "Right", ""}, false);
        if(selection == "L1")
            btn = ControllerDigital::L1;
        else if(selection == "L2")
            btn = ControllerDigital::L2;
        else if(selection == "R1")
            btn = ControllerDigital::R1;
        else if(selection == "R2")
            btn = ControllerDigital::R2;
        else if(selection == "A")
            btn = ControllerDigital::A;
        else if(selection == "B")
            btn = ControllerDigital::B;
        else if(selection == "X")
            btn = ControllerDigital::X;
        else if(selection == "Y")
            btn = ControllerDigital::Y;
        else if(selection == "Up")
            btn = ControllerDigital::up;
        else if(selection == "Down")
            btn = ControllerDigital::down;
        else if(selection == "Left")
            btn = ControllerDigital::left;
        else if(selection == "right")
            btn = ControllerDigital::right;
    }
    
    std::vector<std::string> options;

    for(auto& action : motor_controller_map[btn]){
        options.push_back(action.title());
        options.push_back("\n");
    }
    options.push_back(SYMBOL_PLUS " New Action");
    options.push_back("");

    std::string option = button_matrix_page(m, options);
    
    if(option == SYMBOL_PLUS " New Action")
        m.setCallback(newAction);
    else if(option == "Exit")
        m.setCallback(mainMenu);

    for(auto& action : motor_controller_map[btn])
        if(option == action.title())
            m.setCallback(editMotorAction, &action);
}