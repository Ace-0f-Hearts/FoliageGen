//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_IMPORTER_H
#define PROCEDURALFOLIAGEGENERATOR_IMPORTER_H
#include <fstream>
#include <filesystem>

#include "orienteering/map.h"

class Importer
{
public:
    Importer() = delete;
    Importer(const std::filesystem::path& path, Orienteering::Map map);
    Importer(const Importer&) = delete;
    Importer(Importer&&) = delete;

    virtual ~Importer();

    bool DoImport();
protected:
    virtual bool ImportImplementation();

    std::filesystem::path path_;
    Orienteering::Map map_;
    std::ifstream input_stream_;
};


#endif //PROCEDURALFOLIAGEGENERATOR_IMPORTER_H