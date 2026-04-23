//
// Created by ace on 2026-01-08.
//

#include <foliage/json_writer.h>
#include <fstream>
#include <loguru.hpp>

#include "json/writer.h"

void JsonWriter::Run(const std::filesystem::path& output_file, const Json::Value& data)
{
    std::ofstream of_stream;

    Json::StreamWriterBuilder w_builder;
    std::string output_config = Json::writeString(w_builder, data);

    of_stream.open(output_file);

    if (of_stream.is_open())
    {
        of_stream << output_config;
        of_stream.close();
    }
    else
    {
        LOG_S(ERROR) << ("Failed to open output file for writing");
        throw std::runtime_error("Failed to open output file for writing");
    }
}
