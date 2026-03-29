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
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace Spatial
{
    template <unsigned int N>
    class SpatialCoordinate
    {
    public:
        SpatialCoordinate();
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
        SpatialCoordinate operator*(float n) const;
        SpatialCoordinate operator/(float n) const;
        bool operator==(SpatialCoordinate other) const;
        void operator+=(SpatialCoordinate other);
        void operator-=(SpatialCoordinate other);
        void operator*=(float mul);
        void operator/=(float div);
        SpatialCoordinate& operator=(std::initializer_list<float> values);
        SpatialCoordinate& operator=(const SpatialCoordinate& other);

        [[nodiscard]] float DistanceTo(SpatialCoordinate other) const;

    private:
        float coordinates_[N];
    };



    template <unsigned int N>
    SpatialCoordinate<N>::SpatialCoordinate() : coordinates_{}
    {
        for (int i = 0; i < N; i++)
        {
            coordinates_[i] = 0.0f;
        }
    }

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
    float& SpatialCoordinate<N>::operator[](int i)
    { return coordinates_[i]; }

    template <unsigned int N>
    float SpatialCoordinate<N>::operator[](int i) const
    { return coordinates_[i]; }

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
        for (unsigned int i = 0; i < N; i++)
            result[i] -= other[i];
        return result;
    }

    template <unsigned int N>
    SpatialCoordinate<N> SpatialCoordinate<N>::operator*(float n) const
    {
        SpatialCoordinate result(*this);
        for (unsigned int i = 0; i < N; i++)
            result[i] *= n;
        return result;
    }

    template <unsigned int N>
    SpatialCoordinate<N> SpatialCoordinate<N>::operator/(float n) const
    {
        SpatialCoordinate result(*this);
        for (unsigned int i = 0; i < N; i++)
            result[i] /= n;
        return result;
    }

    template <unsigned int N>
    bool SpatialCoordinate<N>::operator==(SpatialCoordinate other) const
    {
        bool result = true;
        for (unsigned int i = 0; i < N; i++)
        {
            result = result && (coordinates_[i] == other.coordinates_[i]);
        }
        return result;
    }

    template <unsigned int N>
    void SpatialCoordinate<N>::operator+=(SpatialCoordinate other)
    {
        for (unsigned int i = 0; i < N; i++)
            coordinates_[i] += other.coordinates_[i];
    }

    template <unsigned int N>
    void SpatialCoordinate<N>::operator-=(SpatialCoordinate other)
    {
        for (unsigned int i = 0; i < N; i++)
            coordinates_[i] -= other.coordinates_[i];
    }

    template <unsigned int N>
    void SpatialCoordinate<N>::operator*=(float mul)
    {
        for (unsigned int i = 0; i < N; i++)
            coordinates_[i] *= mul;
    }

    template <unsigned int N>
    void SpatialCoordinate<N>::operator/=(float div)
    {
        for (auto i = 0; i < N; i++)
            coordinates_[i] /= div;
    }

    template <unsigned int N>
    SpatialCoordinate<N>& SpatialCoordinate<N>::operator=(std::initializer_list<float> values)
    {
        int i = 0;
        for (auto value : values)
        {
            coordinates_[i] = value;
            ++i;
        }
        return *this;
    }

    template <unsigned int N>
    SpatialCoordinate<N>& SpatialCoordinate<N>::operator=(const SpatialCoordinate& other)
    {
        for (auto i = 0; i < N; i++)
        {
            coordinates_[i] = other.coordinates_[i];
        }
        return *this;
    }

    template <unsigned int N>
    float SpatialCoordinate<N>::DistanceTo(SpatialCoordinate other) const
    {
        float result = 0;
        for (auto i = 0; i < N; i++)
        {
            auto temp = other.coordinates_[i] - coordinates_[i];
            result += temp * temp;
        }
        return std::sqrt(result);
    }

    using Spatial2D = SpatialCoordinate<2>;
    using Spatial3D = SpatialCoordinate<3>;


    template <unsigned int N>
    float Length(SpatialCoordinate<N> const& a);

    template <unsigned int N>
    float Dot(SpatialCoordinate<N> const& a, SpatialCoordinate<N> const& b);

    template <unsigned int N>
    SpatialCoordinate<N> Normalize(SpatialCoordinate<N> const& a);




    float Dot(Spatial2D const& a, Spatial2D const& b);
    float Dot(Spatial3D const& a, Spatial3D const& b);
    Spatial2D Cross(Spatial2D const& a, Spatial2D const& b);
    Spatial3D Cross(Spatial3D const& a, Spatial3D const& b);
    float Length(Spatial2D const& a);
    float Length(Spatial3D const& a);
    Spatial2D Normalize(Spatial2D const& a);
    Spatial3D Normalize(Spatial3D const& a);

    inline std::ostream& operator<<(std::ostream& os,const Spatial2D& spatial)
    {
        os << spatial[0] << ":" << spatial[1];
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os,const Spatial3D& spatial)
    {
        os << spatial[0] << ":" << spatial[1] << ":" << spatial[2];
        return os;
    }

    const static Spatial3D kUp = Spatial3D{0.f,1.f,0.f};


}
namespace boost::geometry::traits
{
    template<> struct tag<Spatial::Spatial2D> {using type = point_tag;};
    template<> struct dimension<Spatial::Spatial2D> : boost::mpl::int_<2> {};
    template<> struct coordinate_type<Spatial::Spatial2D> {using type = float;};
    template<> struct coordinate_system<Spatial::Spatial2D> {using type = cs::cartesian;};

    template<int I> struct access<Spatial::Spatial2D, I>
    {
        static double get(const Spatial::Spatial2D& spatial)
        {
            if constexpr (I == 0) return spatial[0];
            return spatial[1];
        }
        static void set(Spatial::Spatial2D& spatial, float value)
        {
            if constexpr (I == 0) spatial[0] = value;
            else spatial[1] = value;
        }
    };

    template<int I> struct access<Spatial::Spatial3D, I>
    {
        static double get(const Spatial::Spatial2D& spatial)
        {
            if constexpr (I == 0) return spatial[0];
            if constexpr (I == 1) return spatial[1];
            return spatial[2];
        }
        static void set(Spatial::Spatial2D& spatial, float value)
        {
            if constexpr (I == 0) spatial[0] = value;
            if constexpr (I == 1) spatial[1] = value;
            else spatial[2] = value;
        }
    };


    template<> struct tag<Spatial::Spatial3D> {using type = point_tag;};
    template<> struct dimension<Spatial::Spatial3D> : boost::mpl::int_<2> {};
    template<> struct coordinate_type<Spatial::Spatial3D> {using type = float;};
    template<> struct coordinate_system<Spatial::Spatial3D> {using type = cs::cartesian;};
}



#endif //PROCEDURALFOLIAGEGENERATOR_SPATIAL_COORDINATES_H