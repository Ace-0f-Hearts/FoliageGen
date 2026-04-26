//
// Created by ace on 2026-04-23.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OMAP_IMPORTER_H
#define PROCEDURALFOLIAGEGENERATOR_OMAP_IMPORTER_H
#include <map>
#include <pugixml.hpp>

#include "importer.h"
#include "orienteering/georeferencing.h"

namespace Omap
{

    class OmapImporter : public Importer
    {
    public:
        OmapImporter(const std::filesystem::path& path, const std::shared_ptr<Orienteering::OrienteeringMap>& map,
            const std::vector<SymbolAttribute>& attributes);

        ~OmapImporter() override;
        bool ImportImplementation() override;

        void ReadFile() override;

        void ImportSymbols(pugi::xml_node node);

        bool SetupSymbol(pugi::xml_node node, Symbol* symbol);

        void ImportLineSymbol(pugi::xml_node node);
        void ImportAreaSymbol(pugi::xml_node node);
        void ImportPointSymbol(pugi::xml_node node);

        void ImportObjects(pugi::xml_node node);
        void ImportObject(pugi::xml_node node);

        void ImportParts(pugi::xml_node node);

        void ImportColors(pugi::xml_node node);
        void ImportColor(pugi::xml_node node);


        ObjectCoordinate ConvertCoordinates(std::string);
        std::vector<ObjectCoordinate> ComputeObjectCoordinates(Object* object, const pugi::xml_node object_node);
        void FillPathCoords(PathObject* object, bool is_area, const pugi::xml_node object_node);

    private:

        pugi::xml_document doc_;
        std::map<unsigned int, std::shared_ptr<Symbol>> symbol_index_;
        // std::map<int, std::shared_ptr<MapColor>> color_index_;
        //
        // std::vector<std::shared_ptr<MapColor>> spot_colors_;
    };
}



#endif //PROCEDURALFOLIAGEGENERATOR_OMAP_IMPORTER_H