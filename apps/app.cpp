//
// Created by ace on 2026-01-22.
//
#include "app.h"

#include <loguru.hpp>
#include <random>
#include <string>
#include <utility>
#include <foliage/generator.h>

#include "foliage/foliage_snapshot_maker.h"
#include "foliage/global_settings.h"
#include "foliage/map_writer.h"

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
    height_map_.reset();
    map_.reset();
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
        number_of_attributes_ = attributes.size();
        DLOG_F(INFO,"Species descriptors extracted");

        DLOG_F(INFO,"Height map loading started ");

        if (settings_.height_map_file.has_value())
        {
            try
            {
                CImg<> hm;
                hm.load(settings_.height_map_file.value().c_str());
                height_map_ = std::make_shared<HeightMap>(hm);
            } catch (std::exception& e)
            {
                LOG_S(ERROR) << "Error loading height_map: " << e.what();
            }

        }

        DLOG_F(INFO,"Height map loading finished");


        DLOG_F(INFO,"Symbol set extraction started");

        json_parser_.Run(settings_.symbol_set_file->c_str());
        value = json_parser_.GetAttributes();
        json_parser_.Cleanup();
        auto symbol_set = JsonExtractor::ExtractSymbolAttributes(value);
        DLOG_F(INFO,"Symbol set extracted: %lu", symbol_set.size());

        DLOG_F(INFO,"Diffusion zones extracted");
        //TODO: Implemenent Diffusion Zone parsing

        DLOG_F(INFO,"Map parsing started");
        if (!map_parser_.Run(settings_.ocad_map_file->c_str(), symbol_set))
        {
            terminating_ = true;
            LOG_S(ERROR) << "Map couldn't be parsed. Terminating...";
            return;
        }
        LOG_F(INFO,"Map parsing successful");


        if (map_->GetObjectAmount() == 0)
        {
            terminating_ = true;
            LOG_S(ERROR) << "No object found on the map. Terminating...";
            return;
        }


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
            LOG_S(ERROR) << "Incorrect argument for number of diffusion points: " << settings_.random_diffusion_zones.value() << "\nMust be greater than 0.";
        }


        DLOG_F(INFO,"Generator building started");

        generator_builder_.SeOrienteeringMap(map_);
        generator_builder_.SetHeightMap(height_map_);
        generator_builder_.SetDiffusionZones(zones);
        generator_builder_.SetSpeciesAttributes(attributes);
        generator_builder_.SetDensity(settings_.density);
        generator_builder_.SetAbioticFactor(settings_.abiotic_factor);
        generator_builder_.SetDiffusionFactor(settings_.diffusion_factor);
        generator_builder_.SetRandomInitialClassification(settings_.random_initial_classification);


        if (!generator_builder_.Build())
        {
            terminating_ = true;
            LOG_S(ERROR) << "Error building generator";
            return;
        }

        DLOG_F(INFO,"Generator building successful");
    }
    DLOG_F(INFO,"App initialized");
}

void App::Generate()
{
    if (terminating_)
    {
        return;
    }

    {
        LOG_SCOPE_F(INFO,"Generation started");
        generator_builder_.generator()->Start();

        auto seeds = generator_builder_.generator()->seeds();



        if (settings_.save_foliage_img)
        {
            LOG_S(INFO) << "Saving foliage img";
            auto bbox = map_->GetBoundingBox();
            LOG_S(INFO) << bbox << " : " << bbox.width() << " : " << bbox.height();
            auto snap = FoliageSnapshotMaker(1.f, bbox);
            snap.RasterizeObjects(map_->GetObjectsOfType(PathO | AreaO), bbox);
            snap.RasterizeSeeds(seeds, bbox,number_of_attributes_);
            snap.GetMap().map().normalize(0,255);
            MapWriter::Write(snap.GetMap(),settings_.foliage_img_file.value());
        }

        auto value = JsonBuilder::FromGeneratedDataVec(seeds);
        JsonWriter::Run(*settings_.instances_output_file,value);


        LOG_F(INFO,"Generation successful");
    }

    if (settings_.map_data_output_file.has_value())
    {
        auto offset = map_->GetBoundingBox().min();
        MapData data {map_->GetBoundingBox().width(),map_->GetBoundingBox().height(),offset[0],offset[1]};

        auto value = JsonBuilder::FromMapData(data);
        JsonWriter::Run(*settings_.map_data_output_file,value);
    }

    {
        OutputConfig output_config;
        output_config.height_map = settings_.height_map_file.value_or("");
        output_config.map_data = settings_.map_data_output_file.value();
        output_config.instances = settings_.instances_output_file.value();
        output_config.species = settings_.species_file.value();
        output_config.map_texture = settings_.foliage_img_file.value_or("");

        auto value = JsonBuilder::FromConfig(output_config);
        JsonWriter::Run(*settings_.output_config_file,value);
    }



    DLOG_F(INFO,"Generation ended");
}

void App::Cleanup()
{
    DLOG_F(INFO,"App cleanup started");


    DLOG_F(INFO,"App cleanup successful");
}

