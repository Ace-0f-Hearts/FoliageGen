//
// Created by ace on 2026-01-08.
//

#include "species_attribute_parser.h"

#include <iostream>
#include <loguru.hpp>

using Value = Json::Value;

void SpeciesAttrParser::Parse()
{
    const Json::CharReaderBuilder builder;
    const auto reader = builder.newCharReader();
    reader->parse(content_.c_str(),content_.c_str() + content_.size(),&root_,&err_);
}

void SpeciesAttrParser::Run(const char* filename)
{
    Open(filename);
    Read();
    Parse();
    Validate();
}

Value SpeciesAttrParser::GetAttributes()
{
    return root_;
}

bool SpeciesAttrParser::Validate()
{
    auto isValid = true;

    CHECK_F(isValid = isValid && !root_.empty(),"Empty JSON parsed or empty JSON array.");
    CHECK_F(isValid = isValid && root_.isArray(),"Invalid JSON parsed: Root is not array.");

    for(auto& attr : root_)
    {
        if (!isValid) break;
        CHECK_F(isValid = isValid && attr.isObject(),"Invalid item in JSON.");
        CHECK_F(isValid = isValid && attr.size() == NUMBER_OF_MEMBERS,"Invalid number of members in item.");
    }

    return isValid;
}

void SpeciesAttrParser::Open(const char* filename)
{
    data_file_.open(filename);
    if (!data_file_.is_open())
    {
        LOG_F(ERROR,"%s could not be opened.",filename);
        throw std::runtime_error("Could not open file " + string(filename));
    }
}


void SpeciesAttrParser::Read()
{
    sstream_ << data_file_.rdbuf();
    content_ = sstream_.str();
}

void SpeciesAttrParser::Cleanup()
{
    data_file_.close();
    content_.clear();
    root_.clear();
    err_.clear();
}