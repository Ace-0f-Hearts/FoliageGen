//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SEED_H
#define PROCEDURALFOLIAGEGENERATOR_SEED_H
#include "spatial/spatial_coordinates.h"
#include <cstdint>
using uint8_t = std::uint8_t;


enum SeedFlag
{
    Initial = 0x1,
    Active = 0x2,
    Classified = 0x4,
};

struct Seed
{
    Seed();
    Seed(Spatial::Spatial2D coord = {0,0},float scale = 1.f) : coordinate(coord), scale(scale), flags(0x0), species_id(0)
    {
    };
    Seed(Seed& other) : coordinate(other.coordinate), scale(other.scale), flags(other.flags), species_id(other.species_id) {};
    Seed(const Seed& other) : coordinate(other.coordinate), scale(other.scale), flags(other.flags), species_id(other.species_id) {};
    // Seed(Seed&& other) noexcept : coordinate(std::move(other.coordinate)), scale(other.scale), flags(other.flags), species_id(other.species_id) {};
    [[nodiscard]] bool IsActive() const { return flags & Active; };
    [[nodiscard]] bool IsInitial() const {return flags & Initial; };
    [[nodiscard]] bool IsClassified() const { return flags & Classified; };

    void SetInactive() { flags &= ~Active; };
    void SetActive() { flags |= Active; };


    float DistanceTo(const Seed& other) const
    {
        float dist = coordinate.DistanceTo(other.coordinate);

        return dist - scale - other.scale;
    };
    float DistanceTo(const Spatial::Spatial2D& point)
    {
        float dist = coordinate.DistanceTo(point);
        return dist - scale;
    }

    Spatial::Spatial2D coordinate;
    float scale;
    uint8_t flags;
    uint32_t species_id;


};

inline std::ostream& operator<<(std::ostream& os, const Seed& seed)
{
    os << "(" << seed.coordinate << "|" << seed.scale << "|" << seed.flags << "|" << seed.species_id << ")";
    return os;
}

#endif //PROCEDURALFOLIAGEGENERATOR_SEED_H