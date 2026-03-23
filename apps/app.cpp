//
// Created by ace on 2026-01-22.
//
#include "app.h"

#include <loguru.hpp>
#include <string>
#include <utility>

#include "../src/json_extractor.h"
#include "foliage/generator.h"

using string = std::string;

App::App(std::filesystem::path path_to_description_file, std::filesystem::path path_to_map_file) :
descriptor_file_path_(std::move(path_to_description_file)),
map_file_path_(std::move(path_to_map_file)),
writer_(GeneratedDataWriter()),
json_builder_(JsonBuilder()),
species_attr_parser_(SpeciesAttrParser()),
map_(std::make_shared<OrienteeringMap>()),
map_parser_(map_)
{


}

App::~App()
{
}

App::App(const App& other) :
    descriptor_file_path_(other.descriptor_file_path_),
    map_file_path_(other.map_file_path_),
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
        species_attr_parser_.Run(descriptor_file_path_.c_str());
        auto value = species_attr_parser_.GetAttributes();
        species_attr_parser_.Cleanup();
        DLOG_F(INFO,"JSON value parsed from file");
        auto attributes = JsonExtractor::Extract(value);
        DLOG_F(INFO,"JSON attributes extracted");

        DLOG_F(INFO,"Diffusion zones extracted");
        //TODO: Implemenent Diffusion Zone parsing
        std::vector<DiffusionZone> zones;

        DLOG_F(INFO,"Map parsing started");
        map_parser_.Run(map_file_path_.c_str());
        DLOG_F(INFO,"Map parsing successful");

        DLOG_F(INFO,"Generator building started");

        generator_builder_.SeOrienteeringMap(map_);
        generator_builder_.SetHeightMap(height_map_);
        generator_builder_.SetDiffusionZones(zones);
        generator_builder_.SetSpeciesAttributes(attributes);
        generator_builder_.SetDensity(10.f);
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



        LOG_F(INFO,"Generation successful");

    }

    DLOG_F(INFO,"Generation ended");
}

void App::Cleanup()
{
    DLOG_F(INFO,"App cleanup started");

    output_file_path_.clear();
    DLOG_F(INFO,"App cleanup successful");
}
