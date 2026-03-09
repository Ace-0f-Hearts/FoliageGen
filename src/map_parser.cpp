//
// Created by ace on 2026-01-08.
//

#include "map_parser.h"

#include <loguru.hpp>

void MapParser::Read()
{

    sstream_ << data_file_.rdbuf();
    content_ = sstream_.str();
}

void MapParser::Parse()
{

}

void MapParser::Open(const char* filename)
{
    data_file_.open(filename);
    if (!data_file_.is_open())
    {
        LOG_F(ERROR,"%s could not be opened.",filename);
        throw std::runtime_error("Could not open file " + string(filename));
    }
}

void MapParser::Run(const char* filename)
{
    Open(filename);
    Read();

}