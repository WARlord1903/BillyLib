#ifndef CONDITIONALMACRO_HPP
#define CONDITIONALMACRO_HPP

#include "ControllerBindings/Macro.hpp"
#include <map>
#include <string>

std::string pad_parentheses(std::string);
std::string strip_string(std::string);
std::string get_parentheses_group(std::string, std::string);
std::vector<std::string> split_by_comma(const std::string::iterator&, const std::string::iterator&);
std::vector<unsigned> get_non_quoted_indices(std::string, char, bool = false);
unsigned get_occurances(std::string, std::string);

bool evaluate_statement(std::string, Macro* = nullptr);
double evaluate_value(std::string, Macro* = nullptr);
bool evaluate_condition(std::string, Macro* = nullptr);

class ConditionalMacro : public Macro{
public:
    ConditionalMacro(): Macro() {update_time = false; update_evals = false;};
    ConditionalMacro(const ConditionalMacro& rhs) { update_time = false; update_evals = false; text = rhs.text; }
    std::istream& operator>>(std::istream&) override;
    std::map<std::string, double>& getVars() { return vars; };
    void setVars(std::map<std::string, double>& ivars){ vars = ivars; }
    void setStart(unsigned s) { start_time = s; }
    void setEvalCount(unsigned count) { eval_count = count; }
private:
    unsigned nested_contitionals;
};

#endif