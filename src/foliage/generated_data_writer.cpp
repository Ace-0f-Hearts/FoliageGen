//
// Created by ace on 2026-01-08.
//

#include "../../include/foliage/generated_data_writer.h"

void GeneratedDataWriter::Write(const Json::Value& data)
{
    root_ = data;
    throw std::logic_error("Not implemented yet!");
}
