//
// Created by ace on 2026-03-20.
//
#include <foliage/generator_builder.h>

bool GeneratorBuilder::Ready() const
{
    auto ready = map_ != nullptr && attributes_.size() >= 1;

    return ready;
}

bool GeneratorBuilder::Build()
{
    bool ready;
    if ((ready = Ready()))
        generator_ = std::make_unique<Generator>(map_,height_map_,diffusion_zones_,attributes_,density_,random_initial_classification_);

    return ready;
}

std::unique_ptr<Generator>& GeneratorBuilder::generator()
{
    return generator_;
}
