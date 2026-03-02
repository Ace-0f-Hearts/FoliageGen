//
// Created by ace on 2026-02-02.

#ifndef GENERATOR_GAUSSIAN_H
#define GENERATOR_GAUSSIAN_H
#include <cmath>
#include <numbers>
#include <stdexcept>


/**
 * Defines a Gaussian distribution.
 */
class Gaussian
{
public:
    Gaussian() = delete;
    Gaussian(float peak, float mean, float std_deviation);

    [[nodiscard]] float CalcDistribution(float value) const;

    [[nodiscard]] float peak() const;
    [[nodiscard]] float mean() const;
    [[nodiscard]] float std_deviation() const;

private:
    /**
     * Highest point of the peak of the curve
     */
    float peak_;

    /**
     * Average value
     */
    float mean_;

    /**
     * Standard deviation
     */
    float std_deviation_;
};


#endif //GENERATOR_GAUSSIAN_H
