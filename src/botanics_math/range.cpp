//
// Created by ace on 2026-02-07.
//

#include <botanics_math/range.h>

Range::Range(const float min, const float max) : min_(min), max_(max)
{
    if (min_ > max_)
        throw std::range_error("Max. is smaller than min.");
}

float Range::Normalize(const float value) const
{
    return (value - min_) / (max_ - min_);
}

float Range::MaxDistance() const
{
    return max_ - min_;
}

float Range::min() const
{
    return min_;
}

float Range::max() const
{
    return max_;
}
