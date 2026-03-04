//
// Created by ace on 2026-02-19.
//

#include <filesystem>
#include <ocad/importer.h>

Importer::Importer(const std::filesystem::path& path, std::shared_ptr<Orienteering::Map> map) : path_(path), map_(map)
{
}

Importer::~Importer()
{
}

bool Importer::DoImport()
{

    ImportImplementation();
    return true;
}

bool Importer::ImportImplementation()
{
    return false;
}

