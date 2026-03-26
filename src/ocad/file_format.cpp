//
// Created by ace on 2026-02-18.
//

#include <loguru.hpp>
#include <utility>
#include <ocad/file_format.h>

#include "orienteering/symbol_attribute.h"

FileFormat::FileFormat(std::string name,std::vector<string> extensions) : name_(name),extensions_(std::move(extensions))
{
}


std::unique_ptr<Importer> FileFormat::CreateImporter(std::filesystem::path path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>&
                                                     attributes) const
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

bool FileFormat::operator==(const FileFormat& format) const
{
    return format.name_== this->name_;
}
