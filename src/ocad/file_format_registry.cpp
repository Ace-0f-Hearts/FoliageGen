//
// Created by ace on 2026-02-18.
//

#include "ocad/file_format_registry.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <loguru.hpp>
#include <utility>


#include "orienteering/symbol_attribute.h"
#include "utility/not_implemented_error.h"

FileFormatRegistry::FileFormatRegistry() : file_formats_()
{
    file_formats_.emplace_back(new OcadFileFormat());
    file_formats_.emplace_back(new OmapFileFormat());

}

std::unique_ptr<Importer> FileFormatRegistry::CreateImporter(const std::filesystem::path& path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<
                                                             SymbolAttribute>& attributes) const
{
    auto extension = path.extension().string();
    auto predicate = [extension](const FileFormat* format){return format->extensions().cend() != std::find(format->extensions().cbegin(),format->extensions().cend(),extension);};
    auto format = FindFormat(predicate,extension);

    LOG_IF_S(WARNING,format == nullptr) << "Extension not supported by any format";


    if (!format)
    {
        format = FindFormatForData(path);
    }
    else if (format != FindFormatForData(path))
    {
        return nullptr;
    }

    LOG_IF_S(INFO, format != nullptr) << "Found supported format";

    return format ? format->CreateImporter(path, std::move(map),attributes) : nullptr;
}

std::vector<FileFormat*> FileFormatRegistry::file_formats()
{
    return file_formats_;
}

const FileFormat* FileFormatRegistry::FindFormat(const std::function<bool(const FileFormat*)>& predicate,std::string extension) const
{
    auto found = std::ranges::find_if(file_formats_,predicate);

    return (found != file_formats_.cend()) ? *found : nullptr;
}

const FileFormat* FileFormatRegistry::FindFormatForData(const std::filesystem::path& path) const
{
    std::ifstream input;
    input.open(path.string(), std::ios::in | std::ios::binary);
    if (!input.is_open())
        throw std::invalid_argument("Could not open file");

    char buffer[256];

    auto total_read = int(input.readsome(buffer, 256));

    for (auto format : file_formats_)
    {
        if (format->UnderstandsHeader(buffer, total_read))
        {
            return format;
        }
    }
    return nullptr;
}
