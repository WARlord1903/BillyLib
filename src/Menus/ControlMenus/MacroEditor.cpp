#include "ControllerBindings/ConditionalMacro.hpp"
#include "Globals.hpp"
#include "Graphics/Menu.hpp"
#include <string>
#include <vector>

std::string expr_to_str(std::string s){
    std::string val;
    std::string::size_type pos;
    if(get_non_quoted_indices(s, '=').size() == 1 && (pos = get_non_quoted_indices(s, '=')[0]) < s.length() - 1)
        return "var \"" + s.substr(0, pos) + "\" = " + expr_to_str(s.substr(pos + 1));
    if(get_occurances(s, "EXPR") >= 1){
        val = get_parentheses_group(s, "EXPR");
        std::string match = "EXPR(" + val + ")";
        s.replace(s.find(match), match.length(), val);
    }
    while(!(val = get_parentheses_group(s, "EXPR")).empty()){
        std::string match = "EXPR(" + val + ")";
        s.replace(s.find(match), match.length(), "(" + val + ")");
    }
    while(!(val = get_parentheses_group(s, "ABS(")).empty()){
        std::string match = pad_parentheses("ABS(" + val + ")");
        if(!get_parentheses_group(val, "").empty())
            val = get_parentheses_group(val, "");
        s.replace(s.find(match), match.length(), "| " + val + " |");
    }
    while(!(val = get_parentheses_group(s, "VAR(")).empty()){
        std::string match = "VAR(" + val + ")";
        s.replace(s.find(match), match.length(), "var(" + val + ")");
    }
    return s;
}

std::string str_to_expr(std::string s){
    std::string::size_type pos;
    std::string::size_type last_pos;
    if((pos = s.find("\" = ")) != std::string::npos){
        std::string first = "var \"";
        return s.substr(s.find(first) + first.length(), pos - first.length()) + "=" + str_to_expr(s.substr(s.find("= ") + 2));
    }
    for(int i = 0; i < get_occurances(s, "("); i++){
        pos = s.find_last_of("(", pos);
        s.replace(pos, 1, "EXPR(");
    }
    for(int i = 0; i < get_occurances(s, "|") / 2; i++){
        pos = s.find("|");
        last_pos = s.find_last_of("|");
        std::string sub = s.substr(pos, last_pos - pos);
        if(!get_non_quoted_indices(s, '+').empty() || !get_non_quoted_indices(s, '-').empty() || !get_non_quoted_indices(s, '*').empty() || !get_non_quoted_indices(s, '/').empty() || !get_non_quoted_indices(s, '%').empty() || !get_non_quoted_indices(s, '^').empty()){
            s.replace(pos, 2, "ABS(EXPR(");
            s.replace(last_pos - 1, 2, "))");
        }
        else{
            s.replace(pos, 2, "ABS(");
            s.replace(last_pos - 1, 2, ")");
        }
    }
    s = "EXPR(" + s + ")";
    return s;
}

void macroEditor(Menu &m, void* macro_ptr_param){
    std::cout << expr_to_str("v=EXPR(9 + ABS(EXPR(7 * EXPR(8 * 9))) + 7)") << "\n";
    std::cout << str_to_expr(expr_to_str("v=EXPR(9 + ABS(EXPR(7 * EXPR(8 * 9))) + 7)")) << "\n";
    Macro *macro_ptr = (Macro*) macro_ptr_param;
    if(!macro_ptr){
        std::string macro_id;
        std::vector<std::string> btn_map;
        for(auto& m : macro_map){
            btn_map.push_back(m.first);
            btn_map.push_back("\n");
        }
        btn_map.back() = "";
        macro_ptr = &macro_map[button_matrix_page(m, btn_map)];
    }

    std::vector<std::string> macro_steps;
    std::string line;
    std::stringstream ss(macro_ptr->getText());
    std::string::size_type equal_pos = 0;
    while(std::getline(ss, line)){

    }
}