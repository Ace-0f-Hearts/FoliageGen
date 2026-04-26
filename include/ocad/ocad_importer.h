//
// Created by ace on 2026-02-19.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H

#include <filesystem>
#include <map>
#include <vector>

#include "importer.h"
#include "ocad_helper.h"
#include "ocad_types.h"
#include "ocad_types_v9.h"

#include <orienteering/georeferencing.h>
#include <spatial/object_coordinate.h>
#include <orienteering/point_object.h>

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

        template <class S>
        void ImportPointSymbol(const S& ocad_symbol);
        template <class S>
        void ImportLineSymbol(const S& ocad_symbol);
        template <class S>
        void ImportAreaSymbol(const S& ocad_symbol);



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
        void ImportSpotColor(const std::string& param);


        MapColor* ComputePointColor(std::size_t data_size, const OcadTypesV9::PointSymbolElement* elements);
        template<class S>
        MapColor* ComputeLineColor(const S& ocad_symbol,const OcadTypesV9::LineSymbolGeneric attributes);
        MapColor* ComputeAreaColor(bool fill_on, const OcadTypesV9::AreaSymbolGeneric& ocad_symbol, std::size_t data_size, const OcadTypesV9::PointSymbolElement* elements);

        template<class OcadBaseSymbol>
        bool SetupSymbol(std::shared_ptr<Symbol>ocad_symbol, const OcadBaseSymbol& base);

        void FillPathCoords(PathObject* object, bool is_area, uint32_t num_points,
                    const Generic::OcadCoord* ocad_points);

        void SetPointCoord(PointObject* object, const ObjectCoordinate& ocad_point);
        void SetPointFlags(std::vector<ObjectCoordinate>& object, uint32_t pos, bool is_area, Generic::OcadCoord ocd_point);

        ObjectCoordinate ConvertOcadPoint(const Generic::OcadCoord& ocad_point);

        int ConvertLength(int16_t ocad_length) const;
        int ConvertLength(uint16_t ocad_length) const;
        template <class T, class R>
        R ConvertLength(T ocad_length) const;

        float ConvertOcadAngle(int ocad_angle);
        MapColor* ConvertColor(int ocad_color);



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
        std::map<unsigned int, std::shared_ptr<Symbol>> symbol_index_;
        std::map<int, std::shared_ptr<MapColor>> color_index_;

        std::vector<std::shared_ptr<MapColor>> spot_colors_;
    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_IMPORT_H
