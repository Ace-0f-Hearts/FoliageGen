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

App::App(std::filesystem::path path_to_description_file, std::filesystem::path path_to_map_file, std::filesystem::path symbol_set_file_path) :
descriptor_file_path_(std::move(path_to_description_file)),
map_file_path_(std::move(path_to_map_file)),
symbol_set_file_path_(std::move(symbol_set_file_path)),
writer_(GeneratedDataWriter()),
json_builder_(JsonBuilder()),
json_parser_(JsonParser()),
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
    symbol_set_file_path_(other.symbol_set_file_path_),
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
        json_parser_.Run(descriptor_file_path_.c_str());
        auto value = json_parser_.GetAttributes();
        json_parser_.Cleanup();
        auto attributes = JsonExtractor::ExtractSpeciesAttributes(value);
        DLOG_F(INFO,"Species descriptors extracted");


        DLOG_F(INFO,"Symbol set extraction started");

        json_parser_.Run(symbol_set_file_path_.c_str());
        value = json_parser_.GetAttributes();
        json_parser_.Cleanup();
        auto symbol_set = JsonExtractor::ExtractSymbolAttributes(value);
        DLOG_F(INFO,"Symbol set extracted: %lu", symbol_set.size());

        DLOG_F(INFO,"Diffusion zones extracted");
        //TODO: Implemenent Diffusion Zone parsing
        std::vector<DiffusionZone> zones;

        DLOG_F(INFO,"Map parsing started");
        map_parser_.Run(map_file_path_.c_str(), symbol_set);
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
