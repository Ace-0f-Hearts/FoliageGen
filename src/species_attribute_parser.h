//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_SPECIESATTRPARSER_H
#define GENERATOR_SPECIESATTRPARSER_H

#include <fstream>
#include <json/json.h>

class SpeciesAttrParser
{
    using string = std::string;
    using String = Json::String;
    using Value = Json::Value;

    const unsigned int NUMBER_OF_MEMBERS = 4;
public:

    void Run(const char* filename);
    Value GetAttributes();
    void Cleanup();

private:
    void Read();
    void Parse();
    void Open(const char* filename);

    bool Validate();
    std::ostringstream sstream_;
    std::ifstream data_file_;
    string content_;
    Value root_;
    String err_;

};


#endif //GENERATOR_SPECIESATTRPARSER_H