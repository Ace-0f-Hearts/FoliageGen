//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_PROCGEN_H
#define GENERATOR_PROCGEN_H

#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0

#include <alglib/optimization.h>
#include <spatial/bounding_box.h>
#include <orienteering/object.h>

#include "diffusion_zone.h"
#include "height_map.h"
#include "seed.h"
#include "species_attribute.h"
#include "orienteering/map.h"

using namespace alglib;
class Generator
{
public:
    Generator(const std::shared_ptr<OrienteeringMap>& map, const std::shared_ptr<HeightMap>& height_map, const std::vector<DiffusionZone>& diffusion_zones, const std::vector<SpeciesAttribute>& attributes, float density);

    std::vector<Seed> seeds();
    std::vector<Seed>& seeds_ref();

    size_t amount_of_seeds() const;
    size_t amount_of_active_seeds() const;
    size_t amount_of_inactive_seeds() const;

    void Start();


private:
    void PurgeInactiveSeeds();

    void InitializeSeeds();
    void ChooseInitialSeeds();
    void LabelInitialSeeds();
    void LabelRestOfSeeds();
    void MaximizeCoveredArea();

    std::vector<Seed> InitializeSeedsOnObject(float density, Object& object);
    void Randomize(std::vector<Seed>& seeds, float density, float factor = 1.f, float angle = 0);
    void Cull(std::vector<Seed>& seeds, const Object& area);

    void GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad,void *ptr);

    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;

    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;

    float density_;

    std::vector<Seed> seeds_;
    std::vector<size_t> initial_set_indices_;

};


#endif //GENERATOR_PROCGEN_H