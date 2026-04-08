//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_APP_H
#define GENERATOR_APP_H
#include <string>
#include <optional>

#include <foliage/json_extractor.h>
#include <foliage//json_builder.h>
#include <foliage/map_parser.h>
#include <foliage/json_writer.h>
#include <foliage/json_parser.h>
#include <foliage/generator.h>
#include <foliage/generator_builder.h>


struct Settings
{
    bool help {false};
    bool verbose{false};
    bool random_initial_classification {false};
    std::optional<std::filesystem::path> symbol_set_file;
    std::optional<std::filesystem::path> species_file;
    std::optional<std::filesystem::path> ocad_map_file;
    std::optional<std::filesystem::path> height_map_file;
    std::optional<std::filesystem::path> diffusion_file;
    std::optional<std::filesystem::path> output_file;
    std::optional<int> random_diffusion_zones;
    float abiotic_factor{1.f};
    float diffusion_factor{1.f};
    float max_random_radius{2000.f};
    float min_random_radius{500.f};
};

/*!
 * \brief Handles the entire process of the foliage generation.
 */
class App
{

public:
    App() = delete;
    App(std::filesystem::path path_to_description_file, std::filesystem::path path_to_map_file, std::filesystem::path symbol_set_file_path = "../../example_assets/symbol_attributes/isom_2017.json");
    App(Settings settings);
    App(const App &);
    ~App();
    void Run();
private:
    // const std::filesystem::path descriptor_file_path_;
    // const std::filesystem::path map_file_path_;
    // const std::filesystem::path height_map_file_path_;
    // const std::filesystem::path symbol_set_file_path_;
    //
    // std::filesystem::path output_file_path_;
    Settings settings_;

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