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
#include "orienteering/georeferencing.h"
#include "orienteering/point_object.h"

namespace Ocad
{

    class OcadImporter : public Importer
    {
    public:
        OcadImporter(const std::filesystem::path& path, std::shared_ptr<OrienteeringMap> map,const std::vector<SymbolAttribute>& attributes);

        constexpr static size_t kBuffer_size = 4096 * 8;

    protected:
        bool ImportImplementation() override;

        void ReadFile() override;
        template <class F>
        void ImportImplementation();

        template <class F>
        void ImportSymbols(OcadFile<F>& file);
        template <class O>
        void ImportSymbol(const O& object);

        template <class F>
        void ImportObjects(OcadFile<F>& file);
        template <class O>
        void ImportObject(const O& ocad_object);

        template <class F>
        void ImportGeoreferencing(const OcadFile<F>& file);

        void ImportGeoreferencing(const std::string& param);

        template <class F>
        void ImportColors(const OcadFile<F>& file);

        void ImportColor(const std::string& param);



        bool SetupAreaSymbol(Symbol* symbol, bool fill_on, const Ocad::Generic);

        template<class OcadBaseSymbol>
        bool SetupSymbol(Symbol* ocad_symbol, const OcadBaseSymbol& base);

        void FillPathCoords(PathObject* object, bool is_area, uint32_t num_points,
                    const Generic::OcadCoord* ocad_points);
        void SetPointFlags(std::vector<OcadCoordinate>& object, uint32_t pos, bool is_area, Generic::OcadCoord ocd_point);

        OcadCoordinate ConvertOcadPoint(const Generic::OcadCoord& ocad_point);

        float ConvertOcadAngle(int ocad_angle);
        const MapColor ConvertColor(int ocad_color);



        void TryParamConvert(int& out, const std::string param_value)
        {
            out = std::round(std::stof(param_value));
        }
    protected:
        struct StringHandler
        {
            using Callback = void (OcadImporter::*)(const std::string&);
            int32_t type;
            Callback callback;
        };

        template <class F>
        void HandleStrings(const OcadFile<F>& file, std::initializer_list<StringHandler> handlers);

        std::vector<std::byte> buffer_;

        Georeferencing georef_;

        uint8_t ocad_version_;
        // TODO: Take care of pointer at delete
        std::map<unsigned int, Symbol*> symbol_index_;
    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
