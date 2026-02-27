//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H
#define PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H
#include <filesystem>

#include "importer.h"
#include "orienteering/map.h"

class FileFormat
{
    using string = std::string;
public:
    FileFormat();
    virtual ~FileFormat() = default;

    [[nodiscard]] virtual std::unique_ptr<Importer> CreateImporter(std::filesystem::path path, Orienteering::Map map) const;
    virtual bool UnderstandsHeader(const char *buffer, int total_read) const;

    [[nodiscard]] std::vector<string> extensions() const;
    std::vector<string> extensions();
private:

    std::vector<string> extensions_;
};

#endif //PROCEDURALFOLIAGEGENERATOR_FILE_FORMAT_H