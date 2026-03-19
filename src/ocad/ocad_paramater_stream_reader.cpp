//
// Created by ace on 2026-03-17.
//

#include <loguru.hpp>
#include <ocad/ocad_paramater_stream_reader.h>

Ocad::OcadParameterStreamReader::OcadParameterStreamReader(const std::string& param) noexcept
: param_(param),
pos_(0),
next_(param.find('\t',0)),
current_key_(KNo_key())
{}

bool Ocad::OcadParameterStreamReader::ReadNext()
{
    while (!AtEnd())
    {
        pos_ = next_ + 1;
        next_ = param_.find('\t', pos_);
        if (pos_ < param_.length())
        {
            current_key_ = param_.at(pos_);
            if (current_key_ != '\t')
            {
                ++pos_;
                return true;
            }
        }
    }
    current_key_ = KNo_key();
    pos_ = param_.length();
    return false;
}

std::string Ocad::OcadParameterStreamReader::Value() const
{
    return param_.substr(pos_,next_-pos_);
}
