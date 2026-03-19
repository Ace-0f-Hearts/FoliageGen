//
// Created by ace on 2026-02-16.
//

#include <iostream>
#include <loguru.hpp>
#include <random>
#include <foliage/generation.h>
#include <alglib/optimization.h>

using namespace alglib;

std::vector<Spatial::Spatial2D> Generation::InitializeSeeds(const std::shared_ptr<OrienteeringMap>& map, float density)
{
    auto bounding_box = map->GetBoundingBox();
    std::vector<Spatial2D> all_seeds;

    for (auto& object : map->GetObjects())
    {
        if (object->type() == AreaO)
        {
            auto seeds = InitializeSeedsOnObject(density,*object);
            LOG_F(INFO,"Amount of seeds initialized in object: %lu",seeds.size());
            all_seeds.reserve(all_seeds.size() + seeds.size());
            all_seeds.insert(all_seeds.end(),seeds.begin(),seeds.end());
        }
    }

    return all_seeds;
}

std::vector<Spatial::Spatial2D> Generation::InitializeSeedsOnObject(float density, Object& object)
{
    auto bounding_box = object.bounding_box();
    std::vector<Spatial::Spatial2D> seeds;
    float offset = density / 2;

    size_t seeds_per_row = std::floor(bounding_box.width() / density);
    size_t seeds_per_column = std::floor(bounding_box.height() / density);
    size_t number_of_seeds = std::floor(bounding_box.width() * bounding_box.height() / density);

    if (seeds_per_column == 0 || seeds_per_row == 0 || number_of_seeds == 0)
        return seeds;


    LOG_F(INFO,"\tNumber of seeds: %lu\tSeeds per column: %lu\tSeeds per row: %lu",number_of_seeds,seeds_per_column,seeds_per_row);
    LOG_S(INFO) << bounding_box << " with width and height: " << bounding_box.width() << "x" << bounding_box.height();
    for (size_t i = 0; i < number_of_seeds; i++)
    {
        auto list = {offset + i % seeds_per_row * density,offset + i / seeds_per_column * density};
        seeds.emplace_back(list);
    }
    Randomize(seeds, density);
    // seeds = Cull(seeds, object);
    LOG_F(INFO,"\tNumber of seeds after culling: %lu",seeds.size());

    return seeds;
}

void Generation::Randomize(std::vector<Spatial::Spatial2D>& seeds, float density, float factor,float angle)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    auto half_distance = density / 2 * factor;

    std::normal_distribution<float> dist(0, half_distance);
    for (auto& seed: seeds)
    {

        auto offset = Spatial::Spatial2D({dist(gen),dist(gen)});
        seed += offset;
    }

}

std::vector<Spatial::Spatial2D> Generation::Cull(std::vector<Spatial::Spatial2D>& seeds,  const Object& object)
{
    std::vector<Spatial::Spatial2D> candidates;

    for (auto& seed: seeds)
    {
        if (object.IsIntersecting(seed))
        {
            candidates.emplace_back(seed);
        }
    }
    return candidates;
}

Generation::Generation(const std::shared_ptr<OrienteeringMap>& map, const std::shared_ptr<HeightMap>& height_map,
    const std::vector<DiffusionZone>& diffusion_zones, const std::vector<SpeciesAttribute> attributes_, float density)
{
}

void Generation::ChooseInitialSeeds()
{
}

void Generation::LabelInitialSeeds()
{
}

void Generation::LabelRestOfSeeds()
{
}

void Generation::MaximizeCoveredArea()
{

    double epsg = 1e-7;
    double epsf = 0.0;
    double epsx = 0.0;
    ae_int_t maxits = 0;

    minbleicstate state;
    minbleicreport report;

}

void Generation::GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad,void *ptr)
{
    func = -(pow(x[0],2) + pow(x[1],2));
    grad[0] = -2 * x[0];
    grad[1] = -2 * x[1];
}

