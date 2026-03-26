//
// Created by ace on 2026-02-18.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_FILE_FORMAT_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_FILE_FORMAT_H
#include "file_format.h"

class OcadFileFormat : public FileFormat
{
public:
    OcadFileFormat();
    ~OcadFileFormat() override;
    [[nodiscard]] std::unique_ptr<Importer>  CreateImporter(std::filesystem::path path, std::shared_ptr<Orienteering::OrienteeringMap> map, const std::vector<SymbolAttribute>&
                                                            attributes) const override;
    bool UnderstandsHeader(const char *buffer, int total_read) const override;
};

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_FILE_FORMAT_H