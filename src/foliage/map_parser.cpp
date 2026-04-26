//
// Created by ace on 2026-01-08.
//
#include <loguru.hpp>

#include <foliage/map_parser.h>
#include <ocad/file_format_registry.h>

MapParser::MapParser(std::shared_ptr<Orienteering::OrienteeringMap> map) : map_(map)
{
}

bool MapParser::Run(const std::filesystem::path filename, const std::vector<SymbolAttribute>& attributes)
{
    bool success = true;

    FileFormatRegistry registry;

    auto importer = registry.CreateImporter(filename, map_, attributes);
    if (!importer || !importer->DoImport())
    {
        success = false;
        LOG_S(ERROR) << "Error at import of " << filename.string();
    }
    else
    {
        LOG_F(INFO, "Import successful:\n\tNumber of symbols: %lu\n"
              "\tNumber of colors: %lu\n"
              "\tNumber of objects: %lu\n"
              "\t\tNumber of point objects: %lu\n"
              "\t\tNumber of path objects: %lu\n"
              "\t\tNumber of area objects: %lu\n"
              "\t\tNumber of obstructing area objects: %lu\n"
              "\t\tNumber of vegetation supporting area objects: %lu\n",
              map_->GetSymbolAmount(),
              map_->GetColorsAmount(),
              map_->GetObjectAmount(),
              map_->GetObjectOfTypeAmount(PointO),
              map_->GetObjectOfTypeAmount(PathO),
              map_->GetObjectOfTypeAmount(AreaO),
              map_->GetObstructingObjects().size(),
              map_->GetFreeAreas().size()
        );
        success = true;
    }

    importer.reset();
    LOG_S(INFO) << "Successfully imported map from " << filename.string();
    return success;
}
