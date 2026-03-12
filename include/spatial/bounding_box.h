//
// Created by ace on 2026-02-13.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_BOUNDING_BOX_H
#define PROCEDURALFOLIAGEGENERATOR_BOUNDING_BOX_H

#include <stdexcept>
#include <cmath>
#include <iosfwd>
#include <ostream>
#include <vector>
#include <spatial/spatial_coordinates.h>

template <unsigned int D>
class BoundingBox
{
public:

    BoundingBox();
    BoundingBox(Spatial::SpatialCoordinate<D> min, Spatial::SpatialCoordinate<D> max);

    BoundingBox(const BoundingBox& other);

    float GetRadiusOfSmallestBoundingCircle();

    [[nodiscard]] BoundingBox GetSubspaceOf(Spatial::SpatialCoordinate<D> point) const;

    [[nodiscard]] Spatial::SpatialCoordinate<D> GetCentroid() const;

    /**
     * Calculated shortest distance to point using Euclidean distance
     * @param point
     * @return
     */
    [[nodiscard]] float DistanceTo(Spatial::SpatialCoordinate<D> point) const;

    [[nodiscard]] BoundingBox GetSubspaceByIndex(int index) const;

    static unsigned int dim();
    [[nodiscard]] float diagonal() const;
    [[nodiscard]] float width() const;
    [[nodiscard]] float height() const;
    [[nodiscard]] Spatial::SpatialCoordinate<D> max() const;
    [[nodiscard]] Spatial::SpatialCoordinate<D> min() const;

    bool operator==(const BoundingBox& rhs) const;

private:
    Spatial::SpatialCoordinate<D> max_;
    Spatial::SpatialCoordinate<D> min_;

};

template <unsigned int D>
float BoundingBox<D>::diagonal() const
{
    return std::sqrt(width() * width() + height() * height());
}

template <unsigned int D>
float BoundingBox<D>::width() const
{
    return max_[0] - min_[0];
}

template <unsigned int D>
float BoundingBox<D>::height() const
{
    return max_[1] - min_[1];
}


template <unsigned int D>
BoundingBox<D>::BoundingBox() : max_(), min_()
{
}

template <unsigned int D>
BoundingBox<D>::BoundingBox(Spatial::SpatialCoordinate<D> min, Spatial::SpatialCoordinate<D> max): max_{max}, min_{min}
{
    for (int i = 0; i < D; i++)
    {
        if (min[i] > max[i])
            throw std::invalid_argument("Min. and max. points have component that is min[i] > max[i]" );
    }
}

template <unsigned int D>
BoundingBox<D>::BoundingBox(const BoundingBox& other): max_(other.max_), min_(other.min_)
{}

template <unsigned int D>
float BoundingBox<D>::GetRadiusOfSmallestBoundingCircle()
{
    float result = 0;
    for (int i = 0; i < D; i++)
    {
        auto temp = max_[i] - min_[i];
        result += temp * temp;
    }
    return std::sqrt(result) / 2;
}

template <unsigned int D>
BoundingBox<D> BoundingBox<D>::GetSubspaceOf(Spatial::SpatialCoordinate<D> point) const
{
    auto origin = GetCentroid();

    // auto min = min_;
    // auto max = max_;

    auto index = origin.PositionOther(point);

    return GetSubspaceByIndex(index);

    // for (int  i = 0; i < D; ++i)
    // {
    //     if ((index & 1) == 0)
    //     {
    //         min[i] = origin[i];
    //     }
    //     else
    //     {
    //         max[i] = origin[i];
    //     }
    //     index = index >> 1;
    // }
    //
    // return BoundingBox {min,max};
}

template <unsigned int D>
Spatial::SpatialCoordinate<D> BoundingBox<D>::GetCentroid() const
{
    return (max_ + min_) / 2;
}

template <unsigned int D>
float BoundingBox<D>::DistanceTo(Spatial::SpatialCoordinate<D> point) const
{
    float result = 0;
    for (int i = 0; i < D; i++)
    {
        auto temp = std::max(0.f,std::max(point[i] - max_[i],min_[i]- point[i]));
        result += temp * temp;
    }
    return std::sqrt(result);
}

template <unsigned int D>
BoundingBox<D> BoundingBox<D>::GetSubspaceByIndex(int index) const
{
    auto origin = GetCentroid();
    auto min = this->min();
    auto max = this->max();
    // if (index >= D)
    // {
    //     throw std::invalid_argument("Index out of bounds");
    //
    for (int i = 0; i < D; ++i)
    {

        if ((index & 1) == 0)
        {
            min[i] = origin[i];
        }
        else
        {
            max[i] = origin[i];
        }

        index = index >> 1;
    }
    return BoundingBox {min,max};
}

template <unsigned int D>
unsigned int BoundingBox<D>::dim()
{ return D;}

template <unsigned int D>
Spatial::SpatialCoordinate<D> BoundingBox<D>::max() const
{return max_;}

template <unsigned int D>
Spatial::SpatialCoordinate<D> BoundingBox<D>::min() const
{return min_;}

template <unsigned int D>
bool BoundingBox<D>::operator==(const BoundingBox& rhs) const
{
    return max_ == rhs.max_ && min_ == rhs.min_;
}

using BoundingBox2D = BoundingBox<2>;
using BoundingBox3D = BoundingBox<3>;



inline std::ostream& operator<<(std::ostream& os, const BoundingBox2D& bounding_box)
{
   os << "(" << bounding_box.min() << "," << bounding_box.max() << ")";
    return os;
}
#endif //PROCEDURALFOLIAGEGENERATOR_BOUNDING_BOX_H