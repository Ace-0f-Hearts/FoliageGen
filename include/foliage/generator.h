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

#include <botanics_math/range.h>
#include <botanics_math/gaussian.h>
#include <botanics_math/dim.h>
#include <botanics_math/coord.h>
#include <foliage/diffusion_zone.h>
#include <foliage/height_map.h>
#include <foliage/seed.h>
#include <foliage/species_attribute.h>
#include <orienteering/map.h>

#include <nanoflann.hpp>

#include "foliage_map.h"
#include "map_boundary_calculator.h"
#include "mask.h"


struct LinearConstraint
{
    size_t y;
    float b;
};

struct Constraints
{
    size_t x;
    float box_uc;
    float box_lc;
    std::vector<LinearConstraint> l_const;
};

struct SeedAdaptor
{
    std::vector<Seed> seeds;

    [[nodiscard]] size_t kdtree_get_point_count() const
    {
        return seeds.size();
    }
    [[nodiscard]] float kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        return seeds[idx].coordinate[dim];
    }
    template <class BBOX>
    [[nodiscard]] bool kdtree_get_bbox(BBOX& bbox) const
    {
        return false;
    }

};

struct ObjectWrapper
{

    Object* object;
    bool operator<(ObjectWrapper other) const
    {
        return object->symbol()->GetColor()->GetPriority() < other.object->symbol()->GetColor()->GetPriority();
    };
    bool operator>(ObjectWrapper other) const
    {
        return object->symbol()->GetColor()->GetPriority() > other.object->symbol()->GetColor()->GetPriority();
    };
    bool operator==(ObjectWrapper other) const
    {
        return object->symbol()->GetColor()->GetPriority() == other.object->symbol()->GetColor()->GetPriority();
    };
};


using kd_tree = nanoflann::KDTreeSingleIndexAdaptor<
    nanoflann::L2_Simple_Adaptor<float,SeedAdaptor>,SeedAdaptor,2>;

class Generator
{
public:
    Generator(const std::shared_ptr<OrienteeringMap>& map, const std::shared_ptr<HeightMap>& height_map,
              const std::vector<DiffusionZone>& diffusion_zones, const std::vector<SpeciesAttribute>& attributes,
              float density, bool random_initial_classification = false);

    [[nodiscard]] std::vector<Seed> seeds();
    [[nodiscard]] std::vector<Seed>& seeds_ref();
    [[nodiscard]] std::vector<Seed>& attributes();

    [[nodiscard]] size_t amount_of_seeds() const;
    [[nodiscard]] size_t amount_of_active_seeds() const;
    [[nodiscard]] size_t amount_of_inactive_seeds() const;
    [[nodiscard]] size_t amount_of_classified_seeds() const;


    /**
     * Starts the generation process, resulting in a vector of seeds distributed across the orienteering map
     */
    void Start();

private:
    struct DistributionExtremities
    {
        float min;
        float max;
    };

    void InitializeSeeds();
    [[nodiscard]] std::vector<Seed> InitializeSeedsOnObject(float density, const Object& object);
    [[nodiscard]] std::vector<Seed> InitializeSeedsOnForestAreas(float density);
    void PurgeInactiveSeeds();
    void Randomize(std::vector<Seed>& seeds, float density, float factor = 1.f, float angle = 0) const;
    void Cull(std::vector<Seed>& seeds, const Object& area);
    void CullForestSeeds(std::vector<Seed>& seeds);

    void ChooseInitialSeeds();
    void LabelInitialSeeds();
    void LabelRestOfSeeds();

    void MaximizeCoveredArea();
    void MaximizeCoveredAreaOfSubgraph(std::vector<Constraints>& constraints);
    std::vector<Constraints> ComputeConstraintsForSubGraph(const kd_tree& tree,size_t seed_idx, std::vector<bool>&);
    std::vector<size_t> ProcessSeed(const kd_tree& tree, size_t seed_idx, Constraints& c,std::vector<bool>& seed_bit_map);
    std::vector<double>  MaximizeSeedRadii(std::vector<double> seeds, std::vector<double> box_constr,std::vector<double> bnd_upper,int number_of_rows,int number_of_cols, std::vector<double> lin_constr);

    unsigned int CountSeedOfSpecies(uint32_t idx);

    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;

    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;

    static constexpr float kInitial_percentage{0.2f};
    static constexpr int kKnn_number{3};

    float density_;
    float max_growth_radius_;

    bool do_random_initial_classification_{false};
    bool prefer_larger_plants_{true};

    std::vector<Seed> seeds_;
    std::vector<size_t> initial_set_indices_;

    MaskMaker seed_masker_;
    MapBoundaryCalculator map_boundary_calculator_;
};


#endif //GENERATOR_PROCGEN_H
