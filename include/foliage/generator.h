//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_PROCGEN_H
#define GENERATOR_PROCGEN_H

#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0

#include <spatial/bounding_box.h>
#include <orienteering/object.h>

#include <foliage/diffusion_zone.h>
#include <foliage/height_map.h>
#include <foliage/seed.h>
#include <foliage/species_attribute.h>
#include <orienteering/map.h>



struct Spatial2DAdaptor
{
    std::vector<Spatial2D> points;

    [[nodiscard]] size_t kdtree_get_point_count() const
    {
        return points.size();
    }
    [[nodiscard]] float kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        return points[idx][dim];
    }
    template <class BBOX>
    [[nodiscard]] bool kdtree_get_bbox(BBOX& bbox) const
    {
        return false;
    }

};

class Generator
{
public:
    Generator(const std::shared_ptr<OrienteeringMap>& map, const std::shared_ptr<HeightMap>& height_map,
              const std::vector<DiffusionZone>& diffusion_zones, const std::vector<SpeciesAttribute>& attributes,
              float density);

    std::vector<Seed> seeds();
    std::vector<Seed>& seeds_ref();

    size_t amount_of_seeds() const;
    size_t amount_of_active_seeds() const;
    size_t amount_of_inactive_seeds() const;

    void Start();

private:
    struct DistributionExtremities
    {
        float min;
        float max;
    };

    void PurgeInactiveSeeds();

    void InitializeSeeds();
    void ChooseInitialSeeds();
    void LabelInitialSeeds();
    void LabelRestOfSeeds();
    void MaximizeCoveredArea();
    void MaximizeSeedRadii(std::vector<double> seeds, std::vector<double> box_constr,std::vector<double> bnd_upper, std::vector<double> lin_constr);

    std::vector<Seed> InitializeSeedsOnObject(float density, Object& object);
    void Randomize(std::vector<Seed>& seeds, float density, float factor = 1.f, float angle = 0);
    void Cull(std::vector<Seed>& seeds, const Object& area);


    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;

    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;

    static constexpr float kInitial_percentage = 0.1f;
    static constexpr int kKnn_number = 3;
    float density_;

    std::vector<Seed> seeds_;
    std::vector<size_t> initial_set_indices_;
};


#endif //GENERATOR_PROCGEN_H
