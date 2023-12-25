#ifndef SERIALWRAPPER_HPP
#define SERIALWRAPPER_HPP

#include "main.h"

void read_serial(void*);

class SerialWrapper{
public:
    SerialWrapper(std::uint8_t port): datastream{port}, read_serial_task{read_serial, this, "Serial Reader"} {}
    bool bytes_available() { return datastream.get_read_avail(); }
    std::uint8_t read() { return datastream.read_byte(); }
    void write(std::uint8_t data) { datastream.write_byte(data); }
    void write(std::uint8_t* buf, std::int32_t length) { datastream.write(buf, length); }
    void write(std::string buf) { write((std::uint8_t*) buf.c_str(), buf.size()); }
    void flush() { datastream.flush(); }
    void copy_from(std::istream&);
    void copy_to(std::ostream&);
    std::string getline();
private:
    pros::Serial datastream;
    pros::Task read_serial_task;
};

#endif