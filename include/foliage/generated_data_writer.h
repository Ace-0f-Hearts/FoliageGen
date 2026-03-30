//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_GENERATEDDATAWRITER_H
#define GENERATOR_GENERATEDDATAWRITER_H
#include <fstream>
#include <json/value.h>


class GeneratedDataWriter
{
public:
    void Write(const Json::Value& data);
private:
    std::ofstream of_stream_;
    Json::Value root_;
    Json::String err_;
};


#endif //GENERATOR_GENERATEDDATAWRITER_H