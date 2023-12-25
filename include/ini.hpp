/**
 *
 * @file ini.hpp
 * @author William Reinhardt
 *
 * Contains functionality for reading and writing ini files.
 */

#ifndef INI_H
#define INI_H

#include "main.h"
#include <string>

std::vector<std::string> split_string(std::string, std::string);

class Ini_file{
    public:
        Ini_file(std::string path);
        std::list<std::string> getSections();
        std::list<std::string> getKeys(std::string);
        std::list<std::string> getVals(std::string);
        std::string get(std::string, std::string, std::string);
        void set(std::string, std::string, std::string);
        void erase(std::string section, std::string key) { contents.erase(key); }
        void save();
    private:
        std::string file_path;                                                 //The path that the ini file is located
        std::map<std::string, std::map<std::string, std::string> > contents;    //The contents of the ini file
        void parse_file();
};

#endif
