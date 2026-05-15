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
    bool calculate_map_data_only {false};
    bool save_foliage_img {false};
    std::optional<std::filesystem::path> symbol_set_file;
    std::optional<std::filesystem::path> species_file;
    std::optional<std::filesystem::path> ocad_map_file;
    std::optional<std::filesystem::path> height_map_file;
    std::optional<std::filesystem::path> diffusion_file;
    std::optional<std::filesystem::path> instances_output_file = "./instances.json";
    std::optional<std::filesystem::path> map_data_output_file = "./map_data.json";
    std::optional<std::filesystem::path> output_config_file = "./config.json";
    std::optional<std::filesystem::path> foliage_img_file = "./foliage.jpeg";
    std::optional<int> random_diffusion_zones;
    float abiotic_factor{1.f};
    float diffusion_factor{1.f};
    float max_random_radius{2000.f};
    float min_random_radius{500.f};
    float density{10.f};
};

/*!
 * \brief Handles the entire process of the foliage generation.
 */
class App
{

public:
    App() = delete;
    App(Settings settings);
    App(const App &);
    ~App();
    void Run();
private:
    Settings settings_;
    JsonParser json_parser_;

    std::shared_ptr<Orienteering::OrienteeringMap> map_;
    std::shared_ptr<HeightMap> height_map_;
    MapParser map_parser_;

    GeneratorBuilder generator_builder_;

    size_t number_of_attributes_ = 0;
    bool terminating_{false};

    void Init();
    void Generate();
    void Cleanup();
};


#endif //GENERATOR_APP_H