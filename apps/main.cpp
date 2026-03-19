#include <iostream>
#define LOGURU_WITH_STREAMS 1
#include <loguru/loguru.hpp>
#include "app.h"



int main(int argc,char** args)
{
    {
        loguru::init(argc, args);
        loguru::add_file("all.log",loguru::Append,loguru::Verbosity_MAX);
        loguru::add_file("latest.log",loguru::Truncate,loguru::Verbosity_INFO);
        loguru::g_stderr_verbosity = 1;
    }

    auto app = App(args[1],args[2]);

    app.Run();

    return 0;
}
