#include <iostream>
#define LOGURU_WITH_STREAMS 1
#include <functional>
#include <string>
#include <loguru.hpp>
#include "app.h"



typedef std::function<void(Settings&)> NoArgHandle;
const std::unordered_map<std::string,NoArgHandle> kNo_args {
    {"--help",[](Settings& s){ s.help = true;}},
    {"-h",[](Settings& s){ s.help = true;}},
    {"-v",[](Settings& s){ s.verbose = true;}},
    {"--verbose",[](Settings& s){ s.verbose = true;}},
    {"--quiet",[](Settings& s){ s.verbose = false;}},
    {"--random_init", [](Settings& s){ s.random_initial_classification = true;}},

};

typedef std::function<void(Settings&,const std::string&)> OneArgHandle;
#define S(str,f,v) {str, [](Settings& s, const std::string& arg) {s.f = v;}}
const std::unordered_map<std::string,OneArgHandle> kOne_args {
    S("--height_map",height_map_file,arg),
    S("--ocad",ocad_map_file,arg),
    S("--species",species_file,arg),
    S("--diff",diffusion_file,arg),
    S("--random_diff", random_diffusion_zones,std::stoi(arg)),
    S("--symbol", symbol_set_file,arg),
    S("--out",instances_output_file,arg),
    S("--write_map_data",map_data_output_file,arg),
    {"--write_map_data_only",[](Settings& s,const std::string& arg){ s.calculate_map_data_only = true; s.map_data_output_file = arg;}},
    S("--config",output_config_file,arg),
    {"--foliage_img",[](Settings& s,const std::string& arg){ s.save_foliage_img = true; s.foliage_img_file = arg;}}
};
#undef S

void PrintHelp()
{
    std::cerr << "Flags: \n"
                 "\t -h / --help ~ This description.\n"
                 "\t -v / --verbose ~ Verbose foliage generation. (Does nothing)\n"
                 "\t --quiet ~ No info messages during generation. Default behaviour.\n"
                 "\t --random_diff [INT] ~ Create random diffusion points. Defaults to false.\n"
                 "\t --random_init ~ Assign species to seeds at completely random.\n"
                 "\t --ocad [PATH] ~ Path to .ocad map.\n"
                 "\t --species [PATH] ~ Path to JSON describing the plant species.\n"
                 "\t --height_map [PATH] ~ Path to displacement/height map (Optional)\n"
                 "\t --diff [PATH] ~ Path to JSON describing the diffusion zones map (Optional) (Does nothing)\n"
                 "\t --write_map_data [PATH] ~ Path to where information about the processed will be written to(Optional) \n"
                 "\t --write_map_data_only [PATH] ~ Path to where information about the processed will be written to. Will not generate the foliage. (Optional)\n"
                 "\t --config [PATH] ~ Path to where to write the config file for further visualization. (Optional)\n"

    << std::endl;
}

int main(int argc,char* argv[])
{
    {
        loguru::init(argc, argv);
        loguru::add_file("all.log",loguru::Append,loguru::Verbosity_MAX);
        loguru::add_file("latest.log",loguru::Truncate,loguru::Verbosity_INFO);
        loguru::g_stderr_verbosity = 1;
    }

    Settings settings;

    for (int i = 1; i < argc; ++i)
    {

        string opt {argv[i]};
        if (auto j = kNo_args.find(opt); j != kNo_args.end())
        {
            j->second(settings);
        }
        else if (auto k = kOne_args.find(opt); k != kOne_args.end())
        {
            if (++i < argc)
            {
                k->second(settings,argv[i]);
            }
            else
            {
                throw std::runtime_error("Missing parameter after " + opt);
            }
        }
    }

    if (settings.help)
    {
        PrintHelp();
        return 0;
    }

    if (!settings.species_file.has_value())
    {
        throw std::runtime_error("Species descriptor file missing!");
    }
    else if (!settings.ocad_map_file.has_value())
    {
        throw std::runtime_error("OCAD map file missing.");
    }


    App app(settings);
    app.Run();

    return 0;
}
