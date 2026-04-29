//
// Created by ace on 2026-02-07.
//
#include <botanics_math/gaussian.h>
#include <cmath>
Gaussian::Gaussian(const float peak, const float mean, const float std_deviation) : peak_(peak), mean_(mean),
    std_deviation_(std_deviation)
{
    if (std_deviation_ == 0)
        throw std::invalid_argument("std_deviation must be greater than zero");
}

float Gaussian::CalcDistribution(const float value) const
{
    return peak_ * powf(std::numbers::e, -powf(value - mean_, 2) / powf(std_deviation_, 2));
}

float Gaussian::peak() const
{
    return peak_;
}

float Gaussian::mean() const
{
    return mean_;
}

float Gaussian::std_deviation() const
{
    return std_deviation_;
}
