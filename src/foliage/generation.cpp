//
// Created by ace on 2026-02-16.
//

#include <random>
#include <foliage/generation.h>

std::vector<Spatial2D> Generation::InitializeSeeds(const BoundingBox2D& bounding_box, float density, Object& object)
{
    std::vector<Spatial2D> seeds;
    auto offset = density / 2;

    int seeds_per_row = bounding_box.width() / density;
    int seeds_per_column = bounding_box.height() / density;
    int number_of_seeds = bounding_box.width() * bounding_box.height() / density;

    for (int i = 0; i < number_of_seeds; i++)
    {
        auto list = {offset + (i % seeds_per_row ) * density,offset + (i / seeds_per_column) * density};
        seeds.emplace_back(list);
    }

    Randomize(seeds, density);
    // seeds = Cull(seeds, object.coordinates());

    return seeds;
}

void Generation::Randomize(std::vector<Spatial2D>& seeds, float density, float factor,float angle)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    auto half_distance = density / 2 * factor;

    std::normal_distribution<float> dist(0, half_distance);
    for (auto& seed: seeds)
    {

        auto offset = Spatial2D({dist(gen),dist(gen)});
        seed += offset;
    }

}

std::vector<Spatial2D> Generation::Cull(std::vector<Spatial2D>& seeds, Spatial::PathCollection area)
{
    std::vector<Spatial2D> candidates;

    for (auto& seed: seeds)
    {
        if (area.IsPointInsideArea(seed))
        {
            candidates.emplace_back(seed);
        }
    }
    return candidates;
}
