//
// Created by ace on 2026-02-19.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
#include <filesystem>
#include <map>
#include <vector>

#include "importer.h"
#include "ocad_coordinate.h"
#include "ocad_helper.h"
#include "ocad_types.h"
#include "orienteering/point_object.h"


class Object;

namespace Ocad
{
    class OcadImporter : public Importer
    {
    public:
        OcadImporter(const std::filesystem::path& path, Map& map);

        constexpr static size_t kBuffer_size = 4096 * 8;

    protected:
        bool ImportImplementation() override;

        template <class F>
        void ImportImplementation();

        template <class F>
        void ImportSymbols(OcadFile<F>& file);
        template <class O>
        void ImportSymbol(const O& object);

        template <class F>
        void ImportObjects(OcadFile<F>& file);
        template <class O>
        void ImportObject(const O& object);

        template<class OcadBaseSymbol>
        void SetupSymbol(Symbol* symbol, const OcadBaseSymbol& base);

        static void FillPathCoords(PathObject* object, bool is_area, uint32_t num_points,
                    const Generic::OcadCoord* ocd_points);
        static void SetPointFlags(std::vector<OcadCoordinate>& object, uint32_t pos, bool is_area, Generic::OcadCoord ocd_point);

        static OcadCoordinate ConvertOcadPoint(const Generic::OcadCoord& ocad_point);

    private:

        std::vector<std::byte> buffer_;

        uint8_t ocad_version_;
        std::map<unsigned int, Symbol*> symbol_index_;
    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
