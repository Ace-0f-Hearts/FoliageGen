//
// Created by ace on 2026-01-08.
//

#include <foliage/json_parser.h>

#include <filesystem>
#include <iostream>
#include <loguru.hpp>

using Value = Json::Value;

void JsonParser::Parse()
{
    const Json::CharReaderBuilder builder;
    const auto reader = builder.newCharReader();
    reader->parse(content_.c_str(), content_.c_str() + content_.size(), &root_, &err_);
}

void JsonParser::Run(const char* filename)
{
    Open(filename);
    Read();
    Parse();
    Validate();
}

Value JsonParser::GetAttributes()
{
    return root_;
}

bool JsonParser::Validate()
{
    auto isValid = true;

    CHECK_F(isValid = isValid && !root_.empty(), "Empty JSON parsed or empty JSON array.");

    return isValid;
}

void JsonParser::Open(const char* filename)
{
    data_file_.open(filename);
    if (!data_file_.is_open())
    {
        LOG_S(INFO) << std::filesystem::current_path();
        LOG_F(ERROR, "%s could not be opened.", filename);
        throw std::runtime_error("Could not open file " + string(filename));
    }
}


void JsonParser::Read()
{
    std::ostringstream sstream;
    sstream << data_file_.rdbuf();
    content_ = sstream.str();
}

void JsonParser::Cleanup()
{
    data_file_.close();
    content_.clear();
    root_.clear();
    err_.clear();
}
