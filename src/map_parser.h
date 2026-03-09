//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_MAPPARSER_H
#define GENERATOR_MAPPARSER_H
#include <fstream>
#include <iosfwd>
#include <sstream>

class MapParser
{
    using string = std::string;
public:

    void Run(const char*  filename);

private:
    void Read();
    void Parse();
    void Open(const char* filename);

    void MatchFileFormat(const char* filename);

    std::ostringstream sstream_;
    std::ifstream data_file_;
    string content_;

};




#endif //GENERATOR_MAPPARSER_H
