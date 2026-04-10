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
        if (density <= 0.0f)
            throw std::invalid_argument("Density must be greater than 0.0");
        density_ = density;
    }
    void SetRandomInitialClassification(bool random_initial_classification)
    {
        random_initial_classification_ = random_initial_classification;
    }


    std::unique_ptr<Generator>& generator();
private:
    std::shared_ptr<OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;
    std::vector<DiffusionZone> diffusion_zones_;
    std::vector<SpeciesAttribute> attributes_;
    bool random_initial_classification_{false};
    float density_ = 10.f;

    std::unique_ptr<Generator> generator_;

};
#endif //PROCEDURALFOLIAGEGENERATOR_GENERATOR_BUILDER_H