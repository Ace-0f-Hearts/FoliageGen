//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_APP_H
#define GENERATOR_APP_H
#include <string>

#include "../src/generated_data_writer.h"
#include "../src/json_builder.h"
#include "../src/json_extractor.h"
#include "../src/map_parser.h"
#include <foliage/species_attribute.h>
#include <foliage/json_parser.h>
#include "foliage/generator.h"
#include "foliage/generator_builder.h"


/*!
 * \brief Handles the entire process of the foliage generation.
 */
class App
{

public:
    App() = delete;
    App(std::filesystem::path path_to_description_file, std::filesystem::path path_to_map_file, std::filesystem::path symbol_set_file_path = "/mnt/hobby-partition/Dev/ThesisWork/ProceduralFoliageGenerator/FoliageGen/example_assets/symbol_attributes/isom_2017.json");
    App(const App &);
    ~App();
    void Run();
private:
    const std::filesystem::path descriptor_file_path_;
    const std::filesystem::path map_file_path_;
    const std::filesystem::path height_map_file_path_;
    const std::filesystem::path symbol_set_file_path_;

    std::filesystem::path output_file_path_;

    GeneratedDataWriter writer_;
    JsonBuilder json_builder_;
    JsonParser json_parser_;


    std::shared_ptr<Orienteering::OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;
    MapParser map_parser_;

    GeneratorBuilder generator_builder_;

    void Init();
    void Generate();
    void Cleanup();
};


#endif //GENERATOR_APP_H