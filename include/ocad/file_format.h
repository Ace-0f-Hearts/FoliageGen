//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H
#define PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H
#include <filesystem>

#include "importer.h"
#include "orienteering/map.h"
#include "orienteering/symbol_attribute.h"

class FileFormat
{
public:
    FileFormat() = delete;
    FileFormat(std::string name,std::vector<std::string> extensions);
    virtual ~FileFormat() = default;

    [[nodiscard]] virtual std::unique_ptr<Importer> CreateImporter(std::filesystem::path path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>&
                                                                   attributes) const;
    virtual bool UnderstandsHeader(const char *buffer, int total_read) const;

    std::vector<std::string> const& extensions() const;
    bool operator==(const FileFormat& format) const;

    std::string name() const { return name_;}

protected:

    std::vector<string> extensions_{};
    std::string name_;
};

#endif //PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H