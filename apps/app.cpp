//
// Created by ace on 2026-01-22.
//
#include "app.h"

#include <loguru.hpp>
#include <random>
#include <string>
#include <utility>
#include <foliage/generator.h>

#include "foliage/global_settings.h"

using string = std::string;

App::App(Settings s) :
settings_(s),
json_parser_(JsonParser()),
map_(std::make_shared<OrienteeringMap>()),
map_parser_(map_)
{

}

App::~App()
{
}

App::App(const App& other) :
    settings_(other.settings_),
    map_parser_(other.map_parser_)
{
}

void App::Run()
{
    Init();
    Generate();
    Cleanup();
}

void App::Init()
{
    {
        LOG_SCOPE_F(INFO,"App initialization started");

        DLOG_F(INFO,"Species descriptor parsing started");
        json_parser_.Run(settings_.species_file->c_str());
        auto value = json_parser_.GetAttributes();
        json_parser_.Cleanup();
        auto attributes = JsonExtractor::ExtractSpeciesAttributes(value);
        DLOG_F(INFO,"Species descriptors extracted");


        DLOG_F(INFO,"Symbol set extraction started");

        json_parser_.Run(settings_.symbol_set_file->c_str());
        value = json_parser_.GetAttributes();
        json_parser_.Cleanup();
        auto symbol_set = JsonExtractor::ExtractSymbolAttributes(value);
        DLOG_F(INFO,"Symbol set extracted: %lu", symbol_set.size());

        DLOG_F(INFO,"Diffusion zones extracted");
        //TODO: Implemenent Diffusion Zone parsing

        DLOG_F(INFO,"Map parsing started");
        map_parser_.Run(settings_.ocad_map_file->c_str(), symbol_set);
        DLOG_F(INFO,"Map parsing successful");

        std::vector<DiffusionZone> zones;
        if (settings_.random_diffusion_zones.value() > 0)
        {

            std::random_device rdx, rdy, rdi, rdr;
            std::mt19937 genx(rdx()),geny(rdy()), geni(rdi()), genr(rdr());
            std::uniform_real_distribution<> dist_x(0,map_->GetBoundingBox().width());
            std::uniform_real_distribution<> dist_y(0,map_->GetBoundingBox().height());
            std::uniform_real_distribution<> dist_r(settings_.min_random_radius,settings_.max_random_radius);


            std::uniform_int_distribution<> dist_i(0,attributes.size());

            for (int i = 0; i < settings_.random_diffusion_zones.value(); i++)
            {
                float x = dist_x(genx);
                float y = dist_y(geny);
                float radius = dist_r(genr);
                unsigned int  species_id = dist_i(geni);

                zones.emplace_back(DiffusionZone(Spatial2D{x,y} + map_->GetBoundingBox().min(),radius,species_id));
            }
        } else
        {

        }


        DLOG_F(INFO,"Generator building started");

        generator_builder_.SeOrienteeringMap(map_);
        generator_builder_.SetHeightMap(height_map_);
        generator_builder_.SetDiffusionZones(zones);
        generator_builder_.SetSpeciesAttributes(attributes);
        generator_builder_.SetDensity(10.f);
        generator_builder_.SetRandomInitialClassification(settings_.random_initial_classification);


        if (!generator_builder_.Build())
            throw std::logic_error("Error building generator");

        DLOG_F(INFO,"Generator building successful");
    }
    DLOG_F(INFO,"App initialized");
}

void App::Generate()
{
    {

        LOG_SCOPE_F(INFO,"Generation started");
        generator_builder_.generator()->Start();

        auto seeds = generator_builder_.generator()->seeds();

        auto value = JsonBuilder::FromGeneratedDataVec(seeds);
        settings_.output_file = "./seeds.json";
        JsonWriter::Run(*settings_.output_file,value);

        LOG_F(INFO,"Generation successful");

    }

    DLOG_F(INFO,"Generation ended");
}

void App::Cleanup()
{
    DLOG_F(INFO,"App cleanup started");
    DLOG_F(INFO,"App cleanup successful");
}
