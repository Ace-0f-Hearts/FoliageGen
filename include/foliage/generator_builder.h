//
// Created by ace on 2026-03-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_GENERATOR_BUILDER_H
#define PROCEDURALFOLIAGEGENERATOR_GENERATOR_BUILDER_H
#include "diffusion_zone.h"
#include "generator.h"
#include "height_map.h"
#include "species_attribute.h"
#include "orienteering/map.h"

class GeneratorBuilder
{
public:
    GeneratorBuilder() = default;

    bool Ready() const;

    bool Build();

    void SeOrienteeringMap(const std::shared_ptr<OrienteeringMap>& map) { map_ = map; }
    void SetHeightMap(const std::shared_ptr<HeightMap>& height_map) { height_map_ = height_map; }
    void SetDiffusionZones(const std::vector<DiffusionZone>& zones) { diffusion_zones_ = zones; }
    void SetSpeciesAttributes(const std::vector<SpeciesAttribute>& attributes) { attributes_ = attributes; }
    void SetDensity(float density)
    {
        assert(density > 0.f);
        density_ = density;
    }
    void SetRandomInitialClassification(bool random_initial_classification)
    {
        random_initial_classification_ = random_initial_classification;
    }

    void SetAbioticFactor(float abiotic_factor)
    {
        assert(abiotic_factor >= 0.f);
        abiotic_factor_ = abiotic_factor;
    }

    void SetDiffusionFactor(float diffusion_factor)
    {
        assert(diffusion_factor >= 0.f);
        diffusion_factor_ = diffusion_factor;
    }

    std::unique_ptr<Generator>& generator();
private:
    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;
    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;
    bool random_initial_classification_{false};
    float density_ = 10.f;
    float abiotic_factor_ = 1.f;
    float diffusion_factor_ = 1.f;

    std::unique_ptr<Generator> generator_;

};
#endif //PROCEDURALFOLIAGEGENERATOR_GENERATOR_BUILDER_H