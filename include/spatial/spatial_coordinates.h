//
// Created by ace on 2026-02-13.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SPATIAL_COORDINATES_H
#define PROCEDURALFOLIAGEGENERATOR_SPATIAL_COORDINATES_H

#include <stdexcept>
#include <cmath>
#include <iosfwd>
#include <ostream>
#include <vector>

template <unsigned int N>
class SpatialCoordinate
{
public:
    SpatialCoordinate(const std::initializer_list<float>& values);
    SpatialCoordinate(SpatialCoordinate& other);
    SpatialCoordinate(const SpatialCoordinate& other);

    ~SpatialCoordinate();

    /**
     * Calculates the space of this spatial coordinate relative to \code other\endcode.
     * @param other
     * @return
     */
    [[nodiscard]] int PositionRelativeTo(const SpatialCoordinate& other) const;

    /**
     * Calculates the spatial space of \code other\endcode relative to this spatial coordinate.
     * @param other
     * @return
     */
    [[nodiscard]] int PositionOther(const SpatialCoordinate& other) const;

    float& operator[](int i);
    float operator[](int i) const;
    SpatialCoordinate operator+(const SpatialCoordinate& other) const;
    SpatialCoordinate operator-(const SpatialCoordinate& other) const;
    SpatialCoordinate operator*(int n) const;
    SpatialCoordinate operator/(int n) const;
    bool operator==(SpatialCoordinate other) const;
    void operator+=(SpatialCoordinate other);
    void operator-=(SpatialCoordinate other);
    void operator*=(int mul);
    void operator/=(int div);

    [[nodiscard]] float Distance(SpatialCoordinate other) const;

private:
    float coordinates_[N];
};

template <unsigned int N>
SpatialCoordinate<N>::SpatialCoordinate(const std::initializer_list<float>& values): coordinates_{}
{
    int i = 0;
    for (auto value : values) coordinates_[i++] = value;
}

template <unsigned int N>
SpatialCoordinate<N>::SpatialCoordinate(SpatialCoordinate& other): coordinates_{}
{
    for (int i = 0; i < N; i++)
    {
        coordinates_[i] = other.coordinates_[i];
    }
}

template <unsigned int N>
SpatialCoordinate<N>::SpatialCoordinate(const SpatialCoordinate& other): coordinates_{}
{
    for (int i = 0; i < N; i++)
    {
        coordinates_[i] = other.coordinates_[i];
    }
}

template <unsigned int N>
SpatialCoordinate<N>::~SpatialCoordinate() = default;

template <unsigned int N>
float& SpatialCoordinate<N>::operator[](int i)
{ return coordinates_[i]; }

template <unsigned int N>
float SpatialCoordinate<N>::operator[](int i) const
{ return coordinates_[i]; }

template <unsigned int N>
int SpatialCoordinate<N>::PositionRelativeTo(const SpatialCoordinate& other) const
{
    int c = 0;
    for (int i = 0; i < N; i++)
    {
        if (this->operator[](i) < other[i])
            c += 1 << i;
    }
    return c;
}

template <unsigned int N>
int SpatialCoordinate<N>::PositionOther(const SpatialCoordinate& other) const
{
    return other.PositionRelativeTo(*this);
}

template <unsigned int N>
SpatialCoordinate<N> SpatialCoordinate<N>::operator+(const SpatialCoordinate& other) const
{
    SpatialCoordinate result(*this);
    for (int i = 0; i < N; i++)
        result[i] += other[i];
    return result;
}

template <unsigned int N>
SpatialCoordinate<N> SpatialCoordinate<N>::operator-(const SpatialCoordinate& other) const
{
    SpatialCoordinate result(*this);
    for (int i = 0; i < N; i++)
        result[i] -= other[i];
    return result;
}

template <unsigned int N>
SpatialCoordinate<N> SpatialCoordinate<N>::operator*(int n) const
{
    SpatialCoordinate result(*this);
    for (int i = 0; i < N; i++)
        result[i] *= n;
    return result;
}

template <unsigned int N>
SpatialCoordinate<N> SpatialCoordinate<N>::operator/(int n) const
{
    SpatialCoordinate result(*this);
    for (int i = 0; i < N; i++)
        result[i] /= n;
    return result;
}

template <unsigned int N>
bool SpatialCoordinate<N>::operator==(SpatialCoordinate other) const
{
    bool result = true;
    for ( int i = 0; i < N; i++)
    {
        result = result && (coordinates_[i] == other.coordinates_[i]);
    }
    return result;
}

template <unsigned int N>
void SpatialCoordinate<N>::operator+=(SpatialCoordinate other)
{
    for (int i = 0; i < N; i++)
        coordinates_[i] += other.coordinates_[i];
}

template <unsigned int N>
void SpatialCoordinate<N>::operator-=(SpatialCoordinate other)
{
    for (int i = 0; i < N; i++)
        coordinates_[i] -= other.coordinates_[i];
}

template <unsigned int N>
void SpatialCoordinate<N>::operator*=(int mul)
{
    for (int i = 0; i < N; i++)
        coordinates_[i] *= mul;
}

template <unsigned int N>
void SpatialCoordinate<N>::operator/=(int div)
{
    for (int i = 0; i < N; i++)
        coordinates_[i] /= div;
}

template <unsigned int N>
float SpatialCoordinate<N>::Distance(SpatialCoordinate other) const
{
    float result = 0;
    for (int i = 0; i < N; i++)
    {
        auto temp = other.coordinates_[i] - coordinates_[i];
        result += temp * temp;
    }
    return std::sqrt(result);
}

using Spatial2D = SpatialCoordinate<2>;
using Spatial3D = SpatialCoordinate<3>;

inline std::ostream& operator<<(std::ostream& os,const Spatial2D& spatial)
{
    os << spatial[0] << ":" << spatial[1];
    return os;
}


#endif //PROCEDURALFOLIAGEGENERATOR_SPATIAL_COORDINATES_H