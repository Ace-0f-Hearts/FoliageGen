//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_GENERATEDDATAWRITER_H
#define GENERATOR_GENERATEDDATAWRITER_H
#include <filesystem>
#include <json/value.h>


class JsonWriter
{
public:
    static void Run(const std::filesystem::path& output_file, const Json::Value& data);
};


#endif //GENERATOR_GENERATEDDATAWRITER_H