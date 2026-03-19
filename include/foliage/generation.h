//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_PROCGEN_H
#define GENERATOR_PROCGEN_H

#include <alglib/optimization.h>
#include <spatial/bounding_box.h>
#include <spatial/poly_path.h>
#include <orienteering/object.h>

#include "diffusion_zone.h"
#include "height_map.h"
#include "seed.h"
#include "../../src/species_attribute.h"
#include "orienteering/map.h"

using namespace alglib;
class Generation
{
public:

    Generation(const std::shared_ptr<OrienteeringMap>& map, const std::shared_ptr<HeightMap>& height_map, const std::vector<DiffusionZone>& diffusion_zones, std::vector<SpeciesAttribute> attributes_, float density);
    static std::vector<Spatial2D> InitializeSeeds(const std::shared_ptr<OrienteeringMap>& map,float density);
    void ChooseInitialSeeds();
    void LabelInitialSeeds();
    void LabelRestOfSeeds();
    void MaximizeCoveredArea();

private:
    static std::vector<Spatial2D> InitializeSeedsOnObject(float density, Object& object);
    static void Randomize(std::vector<Spatial2D>& seeds, float density, float factor = 1.f, float angle = 0);
    static std::vector<Spatial2D> Cull(std::vector<Spatial2D>& seeds, const Object& area);

    void GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad,void *ptr);

    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;

    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;

    std::vector<Seed> seeds_;
    std::vector<size_t> initial_set_indices_;

};


#endif //GENERATOR_PROCGEN_H