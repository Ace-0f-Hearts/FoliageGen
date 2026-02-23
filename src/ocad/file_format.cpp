//
// Created by ace on 2026-02-18.
//

#include <loguru.hpp>
#include <ocad/file_format.h>

FileFormat::FileFormat()
{
}

std::unique_ptr<Importer> FileFormat::CreateImporter(std::filesystem::path path, Orienteering::Map map) const
{
    LOG_F(WARNING,"Format does not support import");
    return nullptr;
}

bool FileFormat::UnderstandsHeader(const char* buffer, int total_read) const
{
    LOG_F(WARNING,"No header found for format");
    return false;
}

std::vector<FileFormat::string> FileFormat::extensions() const
{
    return extensions_;
}

std::vector<FileFormat::string> FileFormat::extensions()
{
    return extensions_;
}
