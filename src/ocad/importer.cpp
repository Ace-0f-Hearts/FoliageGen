//
// Created by ace on 2026-02-19.
//

#include <filesystem>
#include <ocad/importer.h>

Importer::Importer(const std::filesystem::path& path, std::shared_ptr<OrienteeringMap> map, const std::vector<SymbolAttribute>& attributes) : path_(path), map_(map), attributes_(attributes)
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

