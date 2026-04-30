//
// Created by ace on 2026-02-16.
//
#include <queue>
#include <iostream>
#include <random>

#include <loguru.hpp>
#include <optimization.h> // alglib
#include <nanoflann.hpp>

#include <foliage/generator.h>
#include <foliage/map_writer.h>
#include <foliage/foliage_snapshot_maker.h>

#include <boost/heap/priority_queue.hpp>

#include <optimization.h> // alglib

#include <queue>

#define ASSERT(condition, message) \
do { \
assert(condition && #message); \
} while (0)
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
    return std::ranges::count_if(seeds_.cbegin(), seeds_.cend(), [](const auto& seed) { return seed.IsActive(); });
}

size_t Generator::amount_of_inactive_seeds() const
{
    return std::ranges::count_if(seeds_.cbegin(), seeds_.cend(), [](const auto& seed) { return !seed.IsActive(); });
}

size_t Generator::amount_of_classified_seeds() const
{
    return std::ranges::count_if(seeds_.cbegin(), seeds_.cend(), [](const auto& seed) { return seed.IsClassified(); });
}

void Generator::Start()
{
    auto bbox = map_->GetBoundingBox();


    if (height_map_ && (bbox.width() > height_map_->Width() || bbox.height() > height_map_->Height()))
    {
        LOG_SCOPE_F(INFO,"Resizing height map...");
        auto z = height_map_->map().depth();
        auto c = height_map_->map().spectrum();
        auto interpol = 3;
        this->height_map_->map().resize(static_cast<int>(std::ceilf(bbox.width())),static_cast<int>(std::ceilf(bbox.height())),z,c,interpol);

    }

    {
        LOG_SCOPE_F(INFO, "Forest masking...");
        std::list<Spatial2D> points;
        for (auto obj : map_->GetObjects())
        {
            auto p = obj->GetPoints();
            points.insert(points.end(), p.begin(), p.end());
        }
        map_boundary_calculator_.ComputeAlphaShape(points);

        map_boundary_calculator_.Write("./alpha_shape.svg");
        map_->ClearObjectsOfFlag(Irrelevant);
    }

    {
        LOG_SCOPE_F(INFO, "Masking obstructing objects for intersection tests...");
        seed_masker_.CreateNewMask(map_->GetBoundingBox(), 2, 1);
        seed_masker_.MaskObjects(map_->GetObstructingObjects());
    }

    {
        LOG_SCOPE_F(INFO, "Seeds initialization...");
        InitializeSeeds();

    }


    {
        LOG_SCOPE_F(INFO, "Labeling of initial set of seeds...");

        PurgeInactiveSeeds();
        ChooseInitialSeeds();
        LabelInitialSeeds();

        for (uint32_t i = 0; i < attributes_.size(); i++)
        {
            DLOG_S(INFO) << "Number of " << attributes_[i].name << " seeds: " << CountSeedOfSpecies(i);
        }
    }


    {
        LOG_SCOPE_F(INFO, "Labeling of all other seeds started!");
        LabelRestOfSeeds();
        for (uint32_t i = 0; i < attributes_.size(); i++)
        {
            LOG_S(INFO) << "Number of " << attributes_[i].name << " seeds: " << CountSeedOfSpecies(i);
        }
    }

    LOG_F(INFO,
      "Seeds result: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu\n\tNumber of classified seeds: %lu",
      amount_of_seeds(), amount_of_active_seeds(), amount_of_inactive_seeds(), amount_of_classified_seeds());
    {
        LOG_SCOPE_F(INFO, "Vegetation maximalization");
        MaximizeCoveredArea();
        PurgeInactiveSeeds();
        LOG_F(INFO,
        "Seeds scaled: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu\n\tNumber of classified seeds: %lu",
        amount_of_seeds(), amount_of_active_seeds(), amount_of_inactive_seeds(), amount_of_classified_seeds());
    }

    {
        LOG_SCOPE_F(INFO, "Vegetation maximalization");
        MaximizeCoveredArea();
        PurgeInactiveSeeds();
        LOG_F(INFO,
          "Seeds scaled: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu\n\tNumber of classified seeds: %lu",
          amount_of_seeds(), amount_of_active_seeds(), amount_of_inactive_seeds(), amount_of_classified_seeds());

    }



}

unsigned int Generator::CountSeedOfSpecies(uint32_t idx)
{
    return std::ranges::count_if(seeds(), [idx](const auto& seed) { return seed.species_id == idx; });
}

void Generator::PurgeInactiveSeeds()
{
    seeds_.erase(std::ranges::remove_if(seeds_, [](const auto& seed) { return !seed.IsActive(); }).begin(),
                 seeds_.end());
}


void Generator::InitializeSeeds()
{
    auto bounding_box = map_->GetBoundingBox();

    // boost::heap::priority_queue<ObjectWrapper> object_queue;
    //
    // for (auto object : map_->GetFreeAreas())
    // {
    //     object_queue.push({object});
    // }
    //
    // while (!object_queue.empty())
    // {
    //     auto object = object_queue.top();
    //     object_queue.pop();
    //     auto seeds = InitializeSeedsOnObject(density_, *(object.object),map);
    //
    //     seeds_.reserve(seeds_.size() + seeds.size());
    //     seeds_.insert(seeds_.end(), seeds.begin(), seeds.end());
    // }
    //
    for (auto& object : map_->GetFreeAreas())
    {
        auto seeds = InitializeSeedsOnObject(density_, *object);
        seeds_.reserve(seeds_.size() + seeds.size());
        seeds_.insert(seeds_.end(), seeds.begin(), seeds.end());
    }
    auto seeds = InitializeSeedsOnForestAreas(density_);
    seeds_.reserve(seeds_.size() + seeds.size());
    seeds_.insert(seeds_.end(), seeds.begin(), seeds.end());
}

std::vector<Seed> Generator::InitializeSeedsOnObject(float density, const Object& object)
{
    const auto bounding_box = object.bounding_box();
    std::vector<Seed> seeds;
    const float offset = density / 2;

    const size_t seeds_per_row = std::floor(bounding_box.width() / density);
    const size_t seeds_per_column = std::floor(bounding_box.height() / density);
    const size_t number_of_seeds = std::floor(bounding_box.width() * bounding_box.height() / density);

    if (seeds_per_column == 0 || seeds_per_row == 0 || number_of_seeds == 0)
        return seeds;

    seeds.reserve(number_of_seeds);

    for (size_t i = 0; i < seeds_per_column; i++)
    {
        for (size_t j = 0; j < seeds_per_row; j++)
        {
            Spatial2D list = {offset + j * density, offset + i * density};
            list = bounding_box.min() + list;
            seeds.emplace_back(list);
        }
    }
    Randomize(seeds, density);
    Cull(seeds, object);

    seed_masker_.MaskObject(&object);

    return seeds;
}

std::vector<Seed> Generator::InitializeSeedsOnForestAreas(float density)
{
    const auto bounding_box = map_->GetBoundingBox();
    std::vector<Seed> seeds;
    const float offset = density / 2;

    const size_t seeds_per_row = std::floor(bounding_box.width() / density);
    const size_t seeds_per_column = std::floor(bounding_box.height() / density);
    const size_t number_of_seeds = std::floor(bounding_box.width() * bounding_box.height() / density);

    if (seeds_per_column == 0 || seeds_per_row == 0 || number_of_seeds == 0)
        return seeds;

    seeds.reserve(number_of_seeds);

    for (size_t i = 0; i < seeds_per_column; i++)
    {
        for (size_t j = 0; j < seeds_per_row; j++)
        {
            Spatial2D list = {offset + j * density, offset + i * density};
            list = bounding_box.min() + list;
            seeds.emplace_back(list);
        }
    }
    Randomize(seeds, density);
    CullForestSeeds(seeds);
    return seeds;
}

void Generator::Randomize(std::vector<Seed>& seeds, float density, const float factor,
                          [[maybe_unused]] const float angle) const
{
    std::random_device x_rd;
    std::random_device y_rd;
    std::mt19937 x_gen(x_rd());
    std::mt19937 y_gen(y_rd());

    auto half_distance = density / 2 * factor;

    std::normal_distribution<float> dist(0, half_distance);
    for (auto& seed : seeds)
    {
        auto offset = Spatial::Spatial2D({dist(x_gen), dist(y_gen)});

        auto bbox = map_->GetBoundingBox();

        seed.coordinate += offset;
        if (seed.coordinate[0] < bbox.min()[0])
            seed.coordinate[0] = bbox.min()[0];
        if (seed.coordinate[1] < bbox.min()[1])
            seed.coordinate[1] = bbox.min()[1];
        if (seed.coordinate[0] > bbox.max()[0])
            seed.coordinate[0] = bbox.max()[0];
        if (seed.coordinate[1] > bbox.max()[1])
            seed.coordinate[1] = bbox.max()[1];
    }
}

void Generator::Cull(std::vector<Seed>& seeds, const Object& object)
{
    auto mask = seed_masker_.GetMask().value();
    for (auto& seed : seeds)
    {
        int x, y;
        x = std::round(seed.coordinate[0] - std::round(map_->GetBoundingBox().min()[0])) * mask.resolution_mult();
        y = std::round(seed.coordinate[1] - std::round(map_->GetBoundingBox().min()[1])) * mask.resolution_mult();
        auto is_obstructed = mask.At({x, y}) > 0;

        if (object.IsIntersecting(seed.coordinate) && !is_obstructed)
        {
            seed.SetActive();
        }
    }
}

void Generator::CullForestSeeds(std::vector<Seed>& seeds)
{
    auto mask = seed_masker_.GetMask().value();
    for (auto& seed : seeds)
    {
        int x, y;
        x = std::round(seed.coordinate[0] - std::round(map_->GetBoundingBox().min()[0])) * mask.resolution_mult();
        y = std::round(seed.coordinate[1] - std::round(map_->GetBoundingBox().min()[1])) * mask.resolution_mult();
        auto is_obstructed = mask.At({x, y}) > 0;

        auto is_inside_map = map_boundary_calculator_.IsCoordinateInsideMap(seed.coordinate);
        if (!is_obstructed && is_inside_map)
        {
            seed.SetActive();
        }
    }
}

Generator::Generator(
    const std::shared_ptr<OrienteeringMap>& map,
    const std::shared_ptr<HeightMap>& height_map,
    const std::vector<DiffusionZone>& diffusion_zones,
    const std::vector<SpeciesAttribute>& attributes,
    float density,
    bool random_initial_classification)
    : map_(map),
      height_map_(height_map),
      diffusion_zones_(diffusion_zones),
      attributes_(attributes),
      density_(density),
      max_growth_radius_(std::ranges::max_element(attributes_,
                                                  [](const SpeciesAttribute& attr1, const SpeciesAttribute& attr2)
                                                  {
                                                      return attr1.growth.max() < attr2.growth.max();
                                                  })->growth.max()),
      do_random_initial_classification_(random_initial_classification)
{
}

void Generator::ChooseInitialSeeds()
{
    size_t number_of_seeds = seeds_.size();
    size_t number_of_initial_seeds = number_of_seeds * kInitial_percentage;

    std::random_device init_rd;
    std::mt19937 rng(init_rd());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, number_of_seeds);

    initial_set_indices_.reserve(number_of_initial_seeds);
    for (size_t i = 0; i < number_of_initial_seeds; i++)
    {
        auto idx = dist(rng);
        initial_set_indices_.push_back(idx);
    }

    std::ranges::sort(initial_set_indices_);
    const auto ret = std::ranges::unique(initial_set_indices_);
    initial_set_indices_.erase(ret.begin(), ret.end());
}

void Generator::LabelInitialSeeds()
{
    std::vector<DistributionExtremities> height_extremities_per_species;
    std::vector<DistributionExtremities> slope_extremities_per_species;
    height_extremities_per_species.reserve(attributes_.size());
    slope_extremities_per_species.reserve(attributes_.size());


    if (height_map_)
    {
        /// In order to normalize the height and slope values in the later calculations,
        /// we need to find the highest and lowest height and slope values for each species
        const auto& offset = map_->GetBoundingBox().min();
        for (unsigned int attr_idx = 0; attr_idx < attributes_.size(); attr_idx++)
        {
            /// Initializing height and slope values for each species
            auto& seed = seeds_[0];

            Coord2 seed_coord(std::round(seed.coordinate[0] - offset[0]), std::round(seed.coordinate[1] - offset[1]));


            auto height = height_map_->HeightAt(seed_coord);
            auto slope = height_map_->SlopeAt(seed_coord);
            height_extremities_per_species[attr_idx].min = height;
            height_extremities_per_species[attr_idx].max = height;

            slope_extremities_per_species[attr_idx].min = slope;
            slope_extremities_per_species[attr_idx].max = slope;
        }


        for (unsigned int attr_idx = 0; attr_idx < attributes_.size(); attr_idx++)
        {
            /// Finding the actual maximums for each species

            auto attr = attributes_[attr_idx];
            auto& height_w_prev = height_extremities_per_species[attr_idx];
            auto& slope_w_prev = height_extremities_per_species[attr_idx];


            for (auto& seed_idx : initial_set_indices_)
            {
                auto& seed = seeds_[seed_idx];


                Coord2 seed_coord(std::round(seed.coordinate[0] - offset[0]),
                                  std::round(seed.coordinate[1] - offset[1]));

                const auto height = height_map_->HeightAt(seed_coord);
                const auto slope = height_map_->SlopeAt(seed_coord);

                const auto height_w = attr.elevation.CalcDistribution(height);
                const auto slope_w = attr.slope.CalcDistribution(slope);


                if (height_w_prev.min > height_w)
                {
                    height_w_prev.min = height_w;
                }
                if (height_w_prev.max < height_w)
                {
                    height_w_prev.max = height_w;
                }

                if (slope_w_prev.min > slope_w)
                {
                    slope_w_prev.min = slope_w;
                }
                if (slope_w_prev.max < slope_w)
                {
                    slope_w_prev.max = slope_w;
                }
            }
        }
    }


    for (auto& seed_idx : initial_set_indices_)
    {
        auto& seed = seeds_[seed_idx];
        auto offset = map_->GetBoundingBox().min();
        Coord2 seed_coord(std::round(seed.coordinate[0] - offset[0]), std::round(seed.coordinate[1] - offset[1]));

        size_t max_idx = 0;
        float max_value = 0.f;

        if (do_random_initial_classification_)
        {
            std::random_device rng;
            std::mt19937 gen(rng());
            std::uniform_int_distribution<> dist(0, static_cast<int>(attributes_.size()) - 1);
            seed.species_id = dist(gen);
            seed.flags |= Classified;
            continue;
        }


        for (size_t attr_idx = 0; attr_idx < attributes_.size(); attr_idx++)
        {
            auto attr = attributes_[attr_idx];
            auto height_w_normalized = 1.f;
            auto slope_w_normalized = 1.f;
            auto diff_w = 0.f;

            float height_c = 1.f;
            float slope_c = 1.f;
            float diff_c = 1.f;

            if (height_map_)
            {
                // DLOG_F(INFO,"Height map used");
                auto height_w = attr.elevation.CalcDistribution(height_map_->HeightAt(seed_coord));
                auto slope_w = attr.slope.CalcDistribution(height_map_->SlopeAt(seed_coord));
                height_w_normalized = (height_extremities_per_species[attr_idx].max - height_w) / (
                    height_extremities_per_species[attr_idx].max - height_extremities_per_species[attr_idx].min);
                slope_w_normalized = (slope_extremities_per_species[attr_idx].max - slope_w) / (
                    slope_extremities_per_species[attr_idx].max - slope_extremities_per_species[attr_idx].min);
            }

            for (auto zone : diffusion_zones_)
            {
                if (zone.id == attr_idx)
                {
                    auto dist = seed.coordinate.DistanceTo(zone.center);
                    auto val = 1 - dist / zone.radius;

                    if (diff_w < val)
                        diff_w = val;
                }
            }

            float value = height_w_normalized * height_c + slope_w_normalized * slope_c + diff_w * diff_c;

            if (value > max_value)
            {
                max_value = value;
                max_idx = attr_idx;
            }
        }

        seed.species_id = max_idx;
        seed.flags |= Classified;
    }
}

void Generator::LabelRestOfSeeds()
{
    std::vector<Seed> initial_seed_cords;
    initial_seed_cords.reserve(initial_set_indices_.size());

    for (auto seed_idx : initial_set_indices_)
    {
        initial_seed_cords.emplace_back(seeds_[seed_idx]);
    }

    SeedAdaptor point_cloud{initial_seed_cords};
    kd_tree initial_tree(2, point_cloud, {10});

    int idx_of_init_set = 0;
    for (size_t seed_idx = 0; seed_idx < seeds_.size(); seed_idx++)
    {
        if (seed_idx == initial_set_indices_[idx_of_init_set])
        {
            idx_of_init_set++;
            continue;
        }

        size_t ret_index[kKnn_number];
        float out_dist_sqr[kKnn_number];
        nanoflann::KNNResultSet<float> result_set(kKnn_number);
        result_set.init(ret_index, out_dist_sqr);

        float query[] = {seeds_[seed_idx].coordinate[0], seeds_[seed_idx].coordinate[1]};
        initial_tree.findNeighbors(result_set, &query[0]);


        std::vector<int> species_counters(attributes_.size(), 0);
        for (size_t idx = 0; idx < result_set.size(); idx++)
        {
            ++species_counters[seeds_[ret_index[idx]].species_id];
        }
        auto majority_idx = std::ranges::distance(species_counters.begin(), std::ranges::max_element(species_counters));
        seeds_[seed_idx].species_id = majority_idx;
        seeds_[seed_idx].flags |= Classified;
    }
}

std::vector<Constraints> Generator::ComputeConstraintsForSubGraph(const kd_tree& tree, size_t seed_idx,
                                                                  std::vector<bool>& seed_bit_map)
{
    std::vector<Constraints> constraints;
    std::queue<size_t> queue;
    queue.push(seed_idx);
    seed_bit_map[seed_idx] = true;
    while (!queue.empty())
    {
        auto idx = queue.front();
        queue.pop();
        Constraints c;

        auto neighbour_indices = ProcessSeed(tree, idx, c, seed_bit_map);


        for (auto n_idx : neighbour_indices)
        {
            if (!seed_bit_map[n_idx])
            {
                seed_bit_map[n_idx] = true;
                queue.push(n_idx);
            }
        }

        constraints.emplace_back(c);
    }

    return constraints;
}

std::vector<size_t> Generator::ProcessSeed(const kd_tree& tree, size_t seed_idx, Constraints& c,
                                           std::vector<bool>& seed_bit_map)
{
    const auto& seed = seeds_[seed_idx];

    std::vector<nanoflann::ResultItem<unsigned, float>> indices_list;

    float query[] = {seeds_[seed_idx].coordinate[0], seeds_[seed_idx].coordinate[1]};

    auto search_radius = attributes_[seed.species_id].growth.max() + max_growth_radius_;
    tree.radiusSearch(query, search_radius * search_radius, indices_list);

    c.x = seed_idx;
    c.box_lc = attributes_[seed.species_id].growth.min();
    c.box_uc = attributes_[seed.species_id].growth.max();

    std::vector<size_t> indices;

    for (const auto& item : indices_list)
    {
        auto idx = item.first; // Seed idx
        auto b = std::sqrt(item.second); // Distance
        if (!seed_bit_map[idx])
        {
            auto a_r = attributes_[seed.species_id].growth.max();
            auto b_r = attributes_[seeds_[idx].species_id].growth.max();

            if (a_r + b_r > b)
            {
                indices.emplace_back(idx);
                c.l_const.emplace_back(idx, b);
            }
        }
    }

    return indices;
}

void Generator::MaximizeCoveredAreaOfSubgraph(std::vector<Constraints>& constraints)
{
    std::vector<double> bnd_lower, bnd_upper, lin_constr, radii;

    bnd_lower.reserve(constraints.size());
    bnd_upper.reserve(constraints.size());
    radii.reserve(constraints.size());

    size_t number_of_linear_constraints = 0;

    size_t x_idx = 0;

    for (auto& c : constraints)
    {
        if (prefer_larger_plants_)
        {
            bnd_lower.emplace_back(0.f);
        }
        else
        {
            bnd_lower.emplace_back(c.box_lc);
        }

        bnd_upper.emplace_back(c.box_uc);

        auto x = c.x;

        // radii.emplace_back((c.box_uc + c.box_lc) / 2.f);
        radii.emplace_back(0.25f);

        number_of_linear_constraints += c.l_const.size();
        for (auto& l : c.l_const)
        {
            auto y = l.y;
            auto b = l.b;
            size_t y_idx = std::ranges::distance(constraints.begin(),
                                                 std::ranges::find_if(constraints, [y](const auto& c)
                                                 {
                                                     return c.x == y;
                                                 }));


            for (size_t idx = 0; idx < constraints.size(); idx++)
            {
                if (idx == x_idx || idx == y_idx)
                    lin_constr.emplace_back(1);
                else
                    lin_constr.emplace_back(0);
            }



            lin_constr.emplace_back(b);
            ++y_idx;
        }
        ++x_idx;
    }

    auto opt_radii = MaximizeSeedRadii(radii, bnd_lower, bnd_upper, number_of_linear_constraints, radii.size() + 1,
                                       lin_constr);
    size_t idx = 0;
    for (const auto& c : constraints)
    {
        auto species = attributes()[seeds_[c.x].species_id];

        if (opt_radii[idx] < species.growth.min())
        {
            seeds_[c.x].SetInactive();
        }
        else
        {
            seeds_[c.x].scale = static_cast<float>(opt_radii[idx]);
        }
        ++idx;
    }
}

void Generator::MaximizeCoveredArea()
{
    SeedAdaptor point_cloud{seeds_};
    kd_tree seed_tree(2, point_cloud, {10});

    std::vector<bool> seed_bit_map(seeds().size(), false);
    size_t idx = 0;


    while (idx < seeds_.size())
    {
        auto constraints = ComputeConstraintsForSubGraph(seed_tree, idx, seed_bit_map);

        MaximizeCoveredAreaOfSubgraph(constraints);
        idx += std::ranges::distance(seed_bit_map.begin() + idx,
                                     std::ranges::find(seed_bit_map.begin() + idx, seed_bit_map.end(), false));
    }
    assert(std::ranges::find(seed_bit_map,false) == seed_bit_map.end());
}

void GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad, void* ptr)
{
    func = 0.;
    for (int i = 0; i < x.length(); ++i)
    {
        func -= pow(x[i], 2) * M_PI;
        grad[i] = -2.f * x[i] * M_PI;
    }
}

std::vector<double> Generator::MaximizeSeedRadii(std::vector<double> radii, std::vector<double> bnd_lower,
                                                 std::vector<double> bnd_upper, int number_of_rows, int number_of_cols,
                                                 std::vector<double> lin_constr)
{
    std::vector<double> opt_radii(radii.size());
    if (lin_constr.empty())
    {
        // Only one seed found in this subgraph, we can maximize the radius
        opt_radii[0] = bnd_upper[0];

        return opt_radii;
    }

    try
    {
        real_1d_array r;
        r.attach_to_ptr(radii.size(), radii.data());

        double epsg = 0;
        double epsf = 0.0;
        double epsx = 0.00001;
        ae_int_t maxits = 0;

        minbleicstate state;
        minbleicreport report;

        minbleiccreate(r, state);

        real_1d_array bndl;
        bndl.attach_to_ptr(static_cast<long>(bnd_lower.size()), bnd_lower.data());
        real_1d_array bndu;
        bndu.attach_to_ptr(static_cast<long>(bnd_upper.size()), bnd_upper.data());

        minbleicsetbc(state, bndl, bndu);

        real_2d_array c;
        c.attach_to_ptr(number_of_rows, number_of_cols, lin_constr.data());

        std::vector<long> ct_vec(number_of_rows, -1);
        integer_1d_array ct;
        ct.setcontent(ct_vec.size(), ct_vec.data());

        minbleicsetlc(state, c, ct);

        minbleicsetcond(state, epsg, epsf, epsx, maxits);
#ifdef DEBUG
        minbleicoptguardsmoothness(state);
        minbleicoptguardgradient(state, 0.0001);
#endif

        alglib::minbleicoptimize(state, GradientFunc);

        real_1d_array out;
        minbleicresults(state, out, report);

        optguardreport ogrep;
        minbleicoptguardresults(state, ogrep);


#ifdef DEBUG
        std::cout << "Termination Type: " << (int)report.terminationtype << std::endl;
        printf("%s\n", ogrep.badgradsuspected ? "true" : "false"); // EXPECTED: false
        printf("%s\n", ogrep.nonc0suspected ? "true" : "false"); // EXPECTED: false
        printf("%s\n", ogrep.nonc1suspected ? "true" : "false"); // EXPECTED: false
#endif

        opt_radii.assign(out.getcontent(), out.getcontent() + out.length());

    }
    catch (alglib::ap_error& e)
    {
        LOG_S(ERROR) << "ALGLIB ERROR: " << e.msg;
        std::cout << "ALGLIB Error: " << e.msg << std::endl;
    }
    return opt_radii;
}

std::vector<SpeciesAttribute>& Generator::attributes()
{
    return attributes_;
}
