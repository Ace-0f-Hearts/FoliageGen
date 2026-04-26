//
// Created by ace on 2026-04-24.
//
#include <cstring>
#include <loguru.hpp>
#include <ocad/omap_file_format.h>

#include "ocad/omap_importer.h"

OmapFileFormat::OmapFileFormat() : FileFormat("OMAP", {".omap"})
{
}

OmapFileFormat::~OmapFileFormat() = default;

std::unique_ptr<Importer> OmapFileFormat::CreateImporter(std::filesystem::path path,
    std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>& symbols) const
{
    LOG_S(INFO) << "Found OMAP file format...";

    return std::make_unique<Omap::OmapImporter>(path, map, symbols);
}

bool OmapFileFormat::UnderstandsHeader(const char* buffer, int total_read) const
{
    bool understands = total_read > 38;

    LOG_IF_S(WARNING,!understands) << "Problem with file format: " << total_read;

    pugi::xml_document doc;
    auto result = doc.load_buffer(buffer, total_read);

    if (result != pugi::status_ok)
    {
        LOG_S(ERROR) << "Error at loading file: " << result;
        understands = false;
    }


    if (understands)
    {
        auto node = doc.child("map");

        understands = understands && !node.empty() && 0 == std::strcmp(node.name(),"map");

        LOG_IF_S(WARNING, !understands) << "Header does not start with map item: " << node.name();

        understands = understands && 0 == std::strcmp(node.attribute("xmlns").value(),"http://openorienteering.org/apps/mapper/xml/v2");
        LOG_IF_S(WARNING, !understands) << "Header does not contain URI";
    }

    return understands;
}
