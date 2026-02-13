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
#include "../src/species_attribute_parser.h"


/*!
 * \brief Handles the entire process of the foliage generation.
 */
class App
{
    using string = std::string;
public:
    App() = delete;
    App(string , string );
    App(const App &);
    ~App();
    void Run();
private:
    const string descriptor_file_path_ = string();
    const string map_file_path_ = string();
    string output_file_path_ = string();

    GeneratedDataWriter writer_;
    JsonBuilder json_builder_;
    SpeciesAttrParser species_attr_parser_;
    MapParser map_parser_;

    void Init();
    void Generate();
    void Cleanup();
};


#endif //GENERATOR_APP_H