//
// Created by ace on 2026-02-02.

#ifndef GENERATOR_RANGE_H
#define GENERATOR_RANGE_H

#include <stdexcept>


/**
 * Defines a range on the set of real numbers.
 */
class Range
{
public:
    Range() = default;
    Range(float min, float max);

    [[nodiscard]] float Normalize(float value) const;
    [[nodiscard]] float MaxDistance() const;

    [[nodiscard]] float min() const;
    [[nodiscard]] float max() const;

private:
    float min_;
    float max_;
};

#endif //GENERATOR_RANGE_H
