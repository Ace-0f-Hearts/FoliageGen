//
// Created by ace on 2026-02-16.
//

#include <iostream>
#include <random>

#include <loguru.hpp>

#include <spatial/quadtree.h>

#include <foliage/generator.h>
#include <foliage/map_writer.h>
#include <foliage/foliage_snapshot_maker.h>

#include <nanoflann.hpp>

#include <optimization.h> // alglib

#include <queue>

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

void Generator::Start()
{
    auto bbox = map_->GetBoundingBox();
    DLOG_S(INFO) << "Forest masking started";
    {
        FoliageMap mask(CImg<>(bbox.width(),bbox.height(),1,3,500.f));
        // FoliageSnapshotMaker::CreateMapMask(mask,map_->GetObjects(),bbox);
        // MapWriter::Write(mask,"../../testing/mask.jpeg");

        map_->ClearObjectsOfFlag(Irrelevant);
    }



    InitializeSeeds();
    LOG_F(INFO, "Seeds initialized: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu",
          amount_of_seeds(), amount_of_active_seeds(), amount_of_inactive_seeds());

    PurgeInactiveSeeds();

    LOG_F(INFO, "Seeds initialized: %lu\n\tNumber of active seeds: %lu\n\tNumber of inactive seeds: %lu",
          amount_of_seeds(), amount_of_active_seeds(), amount_of_inactive_seeds());

    ChooseInitialSeeds();
    LabelInitialSeeds();

    FoliageMap f_map(CImg<>(std::round(bbox.width()),std::round(bbox.height()),1,3,500.f));

    FoliageSnapshotMaker::CreateSnapshot(f_map,map_->GetObjectsOfType(PathO | AreaO), bbox);

    LabelRestOfSeeds();
    MaximizeCoveredArea();

    FoliageSnapshotMaker::CreateSnapshot(f_map,seeds_ref(), bbox, 10);
    MapWriter::Write(f_map);

}

void Generator::PurgeInactiveSeeds()
{
    seeds_.erase(std::ranges::remove_if(seeds_, [](const auto& seed) { return !seed.IsActive(); }).begin(), seeds_.end());
}

void Generator::InitializeSeeds()
{
    auto bounding_box = map_->GetBoundingBox();


    for (auto& object : map_->GetFreeAreas())
    {
        assert(object->type() == AreaO);
        assert(!object->symbol()->IsObstructing());

        auto seeds = InitializeSeedsOnObject(density_, *object);
        // LOG_F(INFO,"Amount of seeds initialized in object: %lu",seeds.size());
        seeds_.reserve(seeds_.size() + seeds.size());
        seeds_.insert(seeds_.end(), seeds.begin(), seeds.end());

    }
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

    return seeds;
}

void Generator::Randomize(std::vector<Seed>& seeds, float density, float factor, float angle)
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
        seed.coordinate += offset;
    }
}

void Generator::Cull(std::vector<Seed>& seeds, const Object& object)
{
    for (auto& seed : seeds)
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
    : map_(map), height_map_(height_map), diffusion_zones_(diffusion_zones), attributes_(attributes), density_(density)
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
}


void Generator::LabelInitialSeeds()
{
    std::vector<DistributionExtremities> height_extremities_per_species;
    std::vector<DistributionExtremities> slope_extremities_per_species;
    height_extremities_per_species.reserve(attributes_.size());
    slope_extremities_per_species.reserve(attributes_.size());


    if (height_map_)
    {
        for (int attr_idx = 0; attr_idx < attributes_.size(); attr_idx++)
        {
            // Initializing seed attributes
            auto& seed = seeds_[0];
            Coord2 seed_coord(std::round(seed.coordinate[0]), std::round(seed.coordinate[1]));

            auto height = height_map_->HeightAt(seed_coord);
            auto slope = height_map_->SlopeAt(seed_coord);
            height_extremities_per_species[attr_idx].min = height;
            height_extremities_per_species[attr_idx].max = height;

            slope_extremities_per_species[attr_idx].min = slope;
            slope_extremities_per_species[attr_idx].max = slope;
        }

        for (int attr_idx = 1; attr_idx < attributes_.size(); attr_idx++)
        {
            auto attr = attributes_[attr_idx];

            for (auto seed_idx : initial_set_indices_)
            {
                auto& seed = seeds_[seed_idx];
                Coord2 seed_coord(std::round(seed.coordinate[0]), std::round(seed.coordinate[1]));

                auto height = height_map_->HeightAt(seed_coord);
                auto slope = height_map_->SlopeAt(seed_coord);

                auto height_w = attr.elevation.CalcDistribution(height);
                auto slope_w = attr.slope.CalcDistribution(slope);


                auto& height_w_prev = height_extremities_per_species[attr_idx];
                auto& slope_w_prev = height_extremities_per_species[attr_idx];

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
        Coord2 seed_coord(std::round(seed.coordinate[0]), std::round(seed.coordinate[1]));

        size_t max_idx = 0;
        float max_value = 0.f;

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
                auto height_w = attr.elevation.CalcDistribution(height_map_->HeightAt(seed_coord));
                auto slope_w = attr.slope.CalcDistribution(height_map_->SlopeAt(seed_coord));
                height_w_normalized = (height_extremities_per_species[attr_idx].max - height_w) / (
                    height_extremities_per_species[attr_idx].max - height_extremities_per_species[attr_idx].min);
                slope_w_normalized = (slope_extremities_per_species[attr_idx].max - height_w) / (
                    slope_extremities_per_species[attr_idx].max - slope_extremities_per_species[attr_idx].min);
            }

            if (diffusion_zones_.size() > 0)
            {
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

    // LOG_S(INFO) << "Initial set size:\t" << initial_set_indices_.size();
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
    kd_tree initial_tree(3,point_cloud,{10});

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
        result_set.init(ret_index,out_dist_sqr);

        float query[] = {seeds_[seed_idx].coordinate[0],seeds_[seed_idx].coordinate[1]};
        initial_tree.findNeighbors(result_set,&query[0]);

        for (size_t idx = 0; idx < kKnn_number; idx++)
        {
            // LOG_S(INFO) << ret_index[idx]  << "\t" << out_dist_sqr[idx];
        }

        std::vector<int> species_counters(attributes_.size());
        for (size_t idx = 0; idx < result_set.size(); idx++)
        {
            ++species_counters[seeds_[ret_index[idx]].species_id];
        }
        auto majority_idx = std::ranges::distance(species_counters.begin(),std::ranges::max_element(species_counters));
        // LOG_S(INFO) << seed_idx << " GOT " << majority_idx;
        seeds()[seed_idx].species_id = majority_idx;
    }
}

std::vector<Constraints> Generator::ComputeConstraintsForSubGraph(const kd_tree& tree,size_t seed_idx,std::vector<bool>& seed_bit_map)
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

        auto neighbour_indices= ProcessSeed(tree,idx,c,seed_bit_map);


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

std::vector<size_t> Generator::ProcessSeed(const kd_tree& tree, size_t seed_idx, Constraints& c,std::vector<bool>& seed_bit_map)
{
    const auto& seed = seeds_[seed_idx];

    std::vector<nanoflann::ResultItem<unsigned,float>> indices_list;

    float query[] = {seeds_[seed_idx].coordinate[0],seeds_[seed_idx].coordinate[1]};


    //TODO: Make this a global value to avoid recalculation
    auto max_growth_radius = std::ranges::max_element(attributes_,[](const SpeciesAttribute& attr1, const SpeciesAttribute& attr2){return attr1.growth.max() < attr2.growth.max();});

    auto search_radius = attributes_[seed.species_id].growth.max() + max_growth_radius->growth.max();

    // DLOG_S(INFO) << "Search radius: " << search_radius;
    tree.radiusSearch(query,search_radius * search_radius,indices_list);

    c.x = seed_idx;
    c.box_lc = 0.f;
    c.box_uc = attributes_[seed.species_id].growth.max();

    std::vector<size_t> indices;

    // DLOG_S(INFO) << "Number of indices found in search radius: " << indices_list.size();

    for (const auto& item : indices_list)
    {

        auto idx = item.first; // Seed idx
        auto b = item.second; // Distance
        if (!seed_bit_map[idx])
        {

            auto a_r = attributes_[seed.species_id].growth.max();
            auto b_r = attributes_[seeds_[idx].species_id].growth.max();
            //
            // DLOG_S(INFO) << "Max growth A: " << a_r;
            // DLOG_S(INFO) << "Max growth B: " << b_r;
            // DLOG_S(INFO) << "Distance: " << b;

            if (a_r + b_r > std::sqrt(b))
            {
                indices.emplace_back(idx);
                c.l_const.emplace_back(idx,b);

            }
        }
    }

    return indices;
}

void Generator::MaximizeCoveredAreaOfSubgraph(std::vector<Constraints>& constraints)
{
    std::vector<double> bnd_lower, bnd_upper, lin_constr;
    std::vector<double> radii(constraints.size(),0.1f); //TODO: Issues mgiht arise from this not being < b

    size_t number_of_linear_constraints = 0;

    for (auto& c : constraints)
    {
        bnd_lower.emplace_back(c.box_lc);
        bnd_upper.emplace_back(c.box_uc);

        auto x = c.x;
        size_t coeff_n = seeds().size();

        number_of_linear_constraints += c.l_const.size();
        for (auto& l : c.l_const)
        {
            auto y = l.y;
            auto b = l.b;

            for (size_t idx = 0; idx < constraints.size(); idx++)
            {

                if (idx == x || idx == y)
                    lin_constr.emplace_back(1);
                else
                    lin_constr.emplace_back(0);
            }

            lin_constr.emplace_back(b);
        }
    }

    auto opt_radii = MaximizeSeedRadii(radii,bnd_lower,bnd_upper,number_of_linear_constraints,radii.size() + 1,lin_constr);

    size_t idx = 0;
    for (const auto& c :  constraints)
    {
        seeds_[c.x].scale = opt_radii[idx++];
    }
}

void Generator::MaximizeCoveredArea()
{

    SeedAdaptor point_cloud{seeds_};
    kd_tree seed_tree(3,point_cloud,{10});

    std::vector<bool> seed_bit_map(seeds().size(),false);
    size_t idx = 0;

    while (idx < seeds_.size())
    {
        DLOG_S(INFO) << "Computing sub graph from index " << idx;

        auto constraints = ComputeConstraintsForSubGraph(seed_tree,idx, seed_bit_map);

        MaximizeCoveredAreaOfSubgraph(constraints);

        idx += std::ranges::distance(seed_bit_map.begin() + idx,std::ranges::find(seed_bit_map.begin() + idx,seed_bit_map.end(),false));
    }
}

void GradientFunc(const real_1d_array& x, double& func, real_1d_array& grad, void* ptr)
{
    func = -(pow(x[0], 2) + pow(x[1], 2));
    grad[0] = -2 * x[0];
    grad[1] = -2 * x[1];
}

std::vector<double> Generator::MaximizeSeedRadii(std::vector<double> radii, std::vector<double> bnd_lower,std::vector<double> bnd_upper,int number_of_rows,int number_of_cols, std::vector<double> lin_constr)
{
    std::vector<double> opt_radii(radii.size());


    DLOG_S(INFO) << "Radii: " << radii.size();
    DLOG_S(INFO) << "Lower Boundary: " << bnd_lower.size();
    DLOG_S(INFO) << "Upper Boundary: " << bnd_upper.size();
    DLOG_S(INFO) << "Linear Constraints: " << lin_constr.size();

    if (lin_constr.size() == 0)
    {
        // Only one seed found in this subgraph
        opt_radii[0] = bnd_upper[0];

        return opt_radii;
    }

    try
    {
        real_1d_array r; r.setcontent(radii.size(),radii.data());
        r.setcontent(radii.size(),radii.data());

        double epsg = 1e-7;
        double epsf = 0.0;
        double epsx = 0.0;
        ae_int_t maxits = 0;

        minbleicstate state;
        minbleicreport report;

        minbleiccreate(r,state);

        real_1d_array bndl; bndl.setcontent(bnd_lower.size(),bnd_lower.data());
        real_1d_array bndu; bndu.setcontent(bnd_upper.size(),bnd_upper.data());


        minbleicsetbc(state,bndl,bndu);

        real_2d_array c; //TODO: Figure out how to init this
        c.setcontent(number_of_rows,number_of_cols,lin_constr.data());

        std::vector<long> ct_vec(number_of_rows,-1);
        integer_1d_array ct;
        ct.setcontent(ct_vec.size(),ct_vec.data());

        minbleicsetlc(state,c,ct);

        minbleicsetcond(state,epsg, epsf, epsx,maxits);

        minbleicoptguardsmoothness(state);
        minbleicoptguardgradient(state,0.001);


        alglib::minbleicoptimize(state,GradientFunc);

        minbleicresults(state,r,report);

        optguardreport ogrep;
        minbleicoptguardresults(state, ogrep);
        printf("%s\n", ogrep.badgradsuspected ? "true" : "false"); // EXPECTED: false
        printf("%s\n", ogrep.nonc0suspected ? "true" : "false"); // EXPECTED: false
        printf("%s\n", ogrep.nonc1suspected ? "true" : "false"); // EXPECTED: false


        opt_radii.assign(r.getcontent(),r.getcontent() + r.length());

    } catch (alglib::ap_error& e)
    {
        std::cout << "ALGLIB Error: " << e.msg << std::endl;
    }
    return opt_radii;
}


