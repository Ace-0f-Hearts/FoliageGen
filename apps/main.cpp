#include <iostream>
#define LOGURU_WITH_STREAMS 1
#include <functional>
#include <string>
#include <loguru.hpp>
#include "app.h"


void SetVerbose();
float ParseFloatArg(const std::string&, const std::string&);
float ParseIntArg(const std::string&, const std::string&);

typedef std::function<void(Settings&)> NoArgHandle;
const std::unordered_map<std::string,NoArgHandle> kNo_args {
    {"--help",[](Settings& s)
    {
        s.help = true;
    }},
    {"-h",[](Settings& s){ s.help = true;}},
    {"-v",[](Settings& s)
    {
        s.verbose = true;
        SetVerbose();
    }},
    {"--verbose",[](Settings& s)
    {
        s.verbose = true;
        SetVerbose();
    }},
    {"--quiet",[](Settings& s){ s.verbose = false;}},
    {"--random_init", [](Settings& s){ s.random_initial_classification = true;}},

};

typedef std::function<void(Settings&,const std::string&)> OneArgHandle;
#define S(str,f,v) {str, [](Settings& s, const std::string& arg) {s.f = v;}}
const std::unordered_map<std::string,OneArgHandle> kOne_args {
    S("--height_map",height_map_file,arg),
    {
        "--density",
        [](Settings& s,const std::string& arg)
        {
            s.density = ParseFloatArg("density", arg);
            CHECK_GT_S(s.density, 0.0f) << "Density must be greater than zero.";
        }
    },
    {
        "--abiotic_factor",
        [](Settings& s,const std::string& arg)
        {
           s.abiotic_factor = ParseFloatArg("abiotic factor", arg);
            CHECK_GE_S(s.abiotic_factor, 0.0f) << "Abiotic factor must be greater or equal to zero";
        }
    },
    {
        "--diffusion_factor",
        [](Settings& s,const std::string& arg)
        {
            s.diffusion_factor = ParseFloatArg("diffusion factor", arg);
            CHECK_GE_S(s.diffusion_factor,0.0f) << "Diffusion factor must be greater or equal to zero";
        }
    },
    S("--ocad",ocad_map_file,arg),
    S("--species",species_file,arg),
    S("--diff",diffusion_file,arg),
    {"--random_diff",[](Settings& s, const std::string& arg)
    {
        s.random_diffusion_zones.emplace(ParseIntArg("amount of random diffusion zones",arg));
        CHECK_GE_S(s.random_diffusion_zones.value(),0.0f) << "Number of random diffusion zones must be greater or equal to zero";
    }
    },
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

void SetVerbose()
{
    loguru::g_stderr_verbosity = 1;
    loguru::add_file("all.log",loguru::Append,loguru::Verbosity_MAX);
    loguru::add_file("latest.log",loguru::Truncate,loguru::Verbosity_INFO);
}

float ParseIntArg(const std::string& arg_name, const std::string& arg_value)
{
    int result = 0;
    try
    {
        result = std::stoi(arg_value);
    } catch (std::invalid_argument& e)
    {
        LOG_S(FATAL) << "Invalid argument for " << arg_name << " : " << arg_value;
    }
    catch (std::out_of_range& e)
    {
        LOG_S(FATAL) << "Argument of " << arg_name << " was out of range: " << arg_value;
    }
    return result;
}

float ParseFloatArg(const std::string& arg_name, const std::string& arg_value)
{
    float result = 0.f;
    try
    {
        result  = std::stof(arg_value);
    } catch (std::invalid_argument& e)
    {
        LOG_S(FATAL) << "Invalid argument for " << arg_name << " : " << arg_value;
    }
    catch (std::out_of_range& e)
    {
        LOG_S(FATAL) << "Argument of " << arg_name << " was out of range: " << arg_value;
    }
    return result;
}

int main(int argc,char* argv[])
{

    loguru::init(argc, argv);
    loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;


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

    {

    }



    App app(settings);
    app.Run();

    return 0;
}
