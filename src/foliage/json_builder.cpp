//
// Created by ace on 2026-01-22.
//

#include <foliage/json_builder.h>

JsonBuilder::JsonBuilder()
{
}

JsonBuilder::Value JsonBuilder::BuildNGet(const GeneratedData&)
{
    throw std::logic_error("Not implemented yet!");
}

void JsonBuilder::Clear()
{
    root_.clear();
    err_.clear();
}
