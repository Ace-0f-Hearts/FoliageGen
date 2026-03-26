//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_IMPORTER_H
#define PROCEDURALFOLIAGEGENERATOR_IMPORTER_H
#include <fstream>
#include <filesystem>

#include "orienteering/map.h"
#include "orienteering/symbol_attribute.h"

class Importer
{
public:
    Importer() = delete;
    Importer(const std::filesystem::path& path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>& attributes);
    Importer(const Importer&) = delete;
    Importer(Importer&&) = delete;

    virtual ~Importer();
    bool DoImport();
protected:
    virtual void ReadFile() = 0;
    virtual bool ImportImplementation();

    std::filesystem::path path_;
    std::shared_ptr<Orienteering::OrienteeringMap> map_;
    std::vector<SymbolAttribute> attributes_;
    std::ifstream input_stream_;
};


#endif //PROCEDURALFOLIAGEGENERATOR_IMPORTER_H