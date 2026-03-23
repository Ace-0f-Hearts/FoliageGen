//
// Created by ace on 2026-02-16.
//

#include <iostream>
#include <loguru.hpp>
#include <random>
#include <foliage/generator.h>
#include <alglib/optimization.h>

#include "foliage/foliage_map_writer.h"
#include "foliage/foliage_snapshot_maker.h"

using namespace alglib;

std::vector<Seed> Generator::seeds()
{
    return seeds_;
}

std::vector<Seed>& Generator::seeds_ref()
{
    return seeds_;
}

size_t Generator::amount_of_seeds() const
{
    return seeds_.size();
}

size_t Generator::amount_of_active_seeds() const
{
    return std::ranges::count_if(seeds_.cbegin(),seeds_.cend(),[](auto seed) { return seed.IsActive();});
}

size_t Generator::amount_of_inactive_seeds() const
{
    return std::ranges::count_if(seeds_.cbegin(),seeds_.cend(),[](auto seed) { return !seed.IsActive();});
}

void Generator::Start()
{
    InitializeSeeds();
    LOG_F(INFO,"Seeds initialized: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu",amount_of_seeds(),amount_of_active_seeds(),amount_of_inactive_seeds());
    LOG_F(INFO,"Seeds initialized: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu",amount_of_seeds(),amount_of_active_seeds(),amount_of_inactive_seeds());

    auto bbox = map_->GetBoundingBox();
    PurgeInactiveSeeds();
    auto f_map = FoliageSnapshotMaker::CreateSnapshot(seeds_ref(),bbox,10);
    FoliageMapWriter::Write(f_map);


    ChooseInitialSeeds();
    LabelInitialSeeds();
    LabelRestOfSeeds();
    MaximizeCoveredArea();
}

void Generator::PurgeInactiveSeeds()
{
    seeds_.erase(std::ranges::remove_if(seeds_,[](auto seed) {return !seed.IsActive();}).begin(),seeds_.end());

}

void Generator::InitializeSeeds()
{

    auto bounding_box = map_->GetBoundingBox();


    for (auto& object : map_->GetObjects())
    {
        if (object->type() == AreaO)
        {
            auto seeds = InitializeSeedsOnObject(density_,*object);
            // LOG_F(INFO,"Amount of seeds initialized in object: %lu",seeds.size());
            seeds_.reserve(seeds_.size() + seeds.size());
            seeds_.insert(seeds_.end(),seeds.begin(),seeds.end());
        }
    }


}

std::vector<Seed> Generator::InitializeSeedsOnObject(float density, Object& object)
{
    auto bounding_box = object.bounding_box();
    std::vector<Seed> seeds;
    float offset = density / 2;

    size_t seeds_per_row = std::floor(bounding_box.width() / density);
    size_t seeds_per_column = std::floor(bounding_box.height() / density);
    size_t number_of_seeds = std::floor(bounding_box.width() * bounding_box.height() / density);

    if (seeds_per_column == 0 || seeds_per_row == 0 || number_of_seeds == 0)
        return seeds;

    seeds.reserve(number_of_seeds);
    // for (size_t i = 0; i < number_of_seeds; i++)
    // {
    //     Spatial2D list =  {offset + i % seeds_per_row * density,offset + i / seeds_per_column * density};
    //     list = bounding_box.min() + list;
    //     seeds.emplace_back(list);
    // }

    for (size_t i = 0; i < seeds_per_column; i++)
    {
        for (size_t j = 0; j < seeds_per_row; j++)
        {
            Spatial2D list =  {offset + j * density,offset + i * density};
            list = bounding_box.min() + list;
            seeds.emplace_back(list);
        }
    }
    Randomize(seeds, density);
    Cull(seeds, object);

    return seeds;
}

void Generator::Randomize(std::vector<Seed>& seeds, float density, float factor,float angle)
{
    std::random_device x_rd;
    std::random_device y_rd;
    std::mt19937 x_gen(x_rd());
    std::mt19937 y_gen(y_rd());

    auto half_distance = density / 2 * factor;

    std::normal_distribution<float> dist(0, half_distance);
    for (auto& seed: seeds)
    {

        auto offset = Spatial::Spatial2D({dist(x_gen),dist(y_gen)});
        seed.coordinate += offset;
    }

}

void Generator::Cull(std::vector<Seed>& seeds,  const Object& object)
{

    for (auto& seed: seeds)
    {
        if (object.IsIntersecting(seed.coordinate))
        {
            seed.flags |= Active;
        }
    }
}

Generator::Generator(
    const std::shared_ptr<OrienteeringMap>& map,
    const std::shared_ptr<HeightMap>& height_map,
    const std::vector<DiffusionZone>& diffusion_zones,
    const std::vector<SpeciesAttribute>& attributes,
    float density)
        : map_(map), height_map_(height_map), diffusion_zones_(diffusion_zones),attributes_(attributes), density_(density)
{
}

void Generator::ChooseInitialSeeds()
{
}

void Generator::LabelInitialSeeds()
{
}

void Generator::LabelRestOfSeeds()
{
}

void Generator::MaximizeCoveredArea()
{

    double epsg = 1e-7;
    double epsf = 0.0;
    double epsx = 0.0;
    ae_int_t maxits = 0;

    minbleicstate state;
    minbleicreport report;

}

void Generator::GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad,void *ptr)
{
    func = -(pow(x[0],2) + pow(x[1],2));
    grad[0] = -2 * x[0];
    grad[1] = -2 * x[1];
}

