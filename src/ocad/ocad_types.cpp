//
// Created by ace on 2026-02-27.
//

#include <limits>
#include <loguru.hpp>
#include <vector>
#include <ocad/ocad_types.h>

bool Ocad::Generic::OcadCoord::IsFirstCurvePoint() const
{
    return x & FlagFirstCurvePoint;
}

bool Ocad::Generic::OcadCoord::IsSecondCurvePoint() const
{
    return x & FlagSecondCurvePoint;
}

bool Ocad::Generic::OcadCoord::NoLeftLine() const
{
    return x & FlagNoLeftLine;
}

bool Ocad::Generic::OcadCoord::IsCornerPoint() const
{
    return y & FlagCornerPoint;
}

bool Ocad::Generic::OcadCoord::IsFirstHolePoint() const
{
    return y & FlagFirstHolePoint;
}

bool Ocad::Generic::OcadCoord::NoRightLine() const
{
    return y & FlagNoRightLine;
}

bool Ocad::Generic::OcadCoord::IsDashPoint() const
{
    return y & FlagDashPoint;
}


const void * GetBlockCheckedRaw(const std::vector<char>& byte_array, uint32_t pos, uint32_t block_size)
{
    if (pos == 0)
    {
        return nullptr;
    }
    else if (uint64_t(pos) + block_size > std::numeric_limits<uint32_t>::max()
                        || pos + block_size - 1 >= uint32_t(byte_array.size()))
    {
        LOG_F(WARNING,"Ocd::getBlockChecked: Requested data block is out of bounds");
        return nullptr;
    }
    return byte_array.data() + pos;
}