//
// Created by ace on 2026-03-17.
//

#include <ocad/ocad_paramater_stream_reader.h>

Ocad::OcadParameterStreamReader::OcadParameterStreamReader(const std::string& param) noexcept
: param_(param),
pos(0),
next(param.find('\t')),
current_key(KNo_key())
{}

bool Ocad::OcadParameterStreamReader::ReadNext()
{
    while (!AtEnd())
    {
        pos = next + 1;
        next = param_.find('\t', next);
        if (pos < param_.length())
        {
            current_key = param_.at(pos);
            if (current_key != '\t')
            {
                ++pos;
                return true;
            }
        }
    }
    current_key = KNo_key();
    pos = param_.length();
    return false;
}

std::string Ocad::OcadParameterStreamReader::Value() const
{
    return param_.substr(pos,std::max(-1,next-pos));
}
