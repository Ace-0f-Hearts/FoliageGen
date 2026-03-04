//
// Created by ace on 2026-02-18.
//

#include <ocad/ocad_file_format.h>

#include "ocad/ocad_importer.h"

OcadFileFormat::OcadFileFormat()
{
}

OcadFileFormat::~OcadFileFormat()
{
}

std::unique_ptr<Importer> OcadFileFormat::CreateImporter(std::filesystem::path path, std::shared_ptr<Map> map) const
{
    return std::make_unique<Ocad::OcadImporter>(path, map);
}

bool OcadFileFormat::UnderstandsHeader(const char* buffer, int total_read) const
{
    if (total_read >= 2 && static_cast<uint8_t>(buffer[0]) == 0xAD && buffer[1] == 0x0C)
        return true;
    return false;
}
