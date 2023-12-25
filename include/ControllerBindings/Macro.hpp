#ifndef MACRO_HPP
#define MACRO_HPP

#include "main.h"
#include <string>

void read_macros(std::string);
void write_macros(std::string);


class Macro{
public:
    Macro(): update_time{true}, eval_count{0} {};
    Macro(const Macro& rhs): update_time{true}, update_evals{true}, eval_count{0}, text{rhs.text} {}
    void run();
    std::ostream& operator<<(std::ostream&) const;
    virtual std::istream& operator>>(std::istream&);
    void waitUntilSettled();
    bool isRunning() { return running; }
    std::string getText() { return text; }
    bool getAsync() { return async; }
    std::uint32_t getStartTime() { return start_time; }
    void setID(std::string iid) { id = iid; }
    void incrementEvalCount() { eval_count++; }
    unsigned getEvalCount() { return eval_count; }
protected:
    std::string id;
    std::string text;
    std::map<std::string, double> vars;
    std::uint32_t start_time;
    unsigned eval_count;
    bool update_time;
    bool update_evals;
    bool async;
    pros::Mutex mutex;
    bool running;
};

std::ostream& operator<<(std::ostream&, const Macro&);
std::istream& operator>>(std::istream&, Macro&);

#endif