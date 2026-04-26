//
// Created by ace on 2026-04-24.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OMAP_FILE_FORMAT_H
#define PROCEDURALFOLIAGEGENERATOR_OMAP_FILE_FORMAT_H
#include "file_format.h"

class OmapFileFormat : public FileFormat
{
public:
    OmapFileFormat();
    ~OmapFileFormat() override;
    [[nodiscard]] std::unique_ptr<Importer> CreateImporter(std::filesystem::path path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>& symbols) const override;

    bool UnderstandsHeader(const char* buffer, int total_read) const override;
};

#endif //PROCEDURALFOLIAGEGENERATOR_OMAP_FILE_FORMAT_H