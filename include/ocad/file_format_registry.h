//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_REGISTRY_H
#define PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_REGISTRY_H
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "ocad_file_format.h"
#include "omap_file_format.h"
#include "orienteering/map.h"
#include "orienteering/symbol_attribute.h"

class FileFormatRegistry
{
    using string = std::string;

public:
    FileFormatRegistry();
    FileFormatRegistry(const FileFormat& file_format) = delete;
    FileFormatRegistry& operator=(FileFormatRegistry) = delete;
    FileFormatRegistry& operator=(FileFormatRegistry const&) = delete;

    [[nodiscard]] std::unique_ptr<Importer>  CreateImporter(const std::filesystem::path& path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<
                                                            SymbolAttribute>& attributes) const;

    std::vector<FileFormat*>file_formats();
private:
    const FileFormat* FindFormat(const std::function<bool (const FileFormat*)>& predicate,std::string) const;
    [[nodiscard]] const FileFormat* FindFormatForData(const std::filesystem::path& path) const;
    std::vector<FileFormat*> file_formats_{};
};

#endif //PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_REGISTRY_H