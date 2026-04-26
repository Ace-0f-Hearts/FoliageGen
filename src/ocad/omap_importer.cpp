//
// Created by ace on 2026-04-23.
//

#include <algorithm>
#include <cstring>
#include <loguru.hpp>
#include <ocad/omap_importer.h>
#include <pugixml.hpp>
#include <boost/container/throw_exception.hpp>
#include <json/value.h>

#include <orienteering/path_object.h>
#include <orienteering/point_object.h>
#include <ocad/omap_helper.h>

using namespace Omap;

OmapImporter::OmapImporter(const std::filesystem::path& path, const std::shared_ptr<Orienteering::OrienteeringMap>& map,
    const std::vector<SymbolAttribute>& attributes) : Importer(path,map,attributes)
{
}

void OmapImporter::ReadFile()
{
    auto file_size = std::filesystem::file_size(path_);

    auto result = doc_.load_file(path_.c_str());

    if (!result)
    {
        LOG_S(ERROR) << "Failed to load OMAP file: " << result;
    }
}

bool OmapImporter::ImportImplementation()
{
    ReadFile();

    auto map = doc_.child("map").child("barrier");

    if (map.empty())
    {
        LOG_S(ERROR) << "Empty OMAP map";
    }

    ImportColors(map.child("colors"));
    ImportSymbols(map.child("symbols"));
    ImportParts(map.child("parts"));

    return true;
}

void OmapImporter::ImportSymbols(pugi::xml_node node)
{
    LOG_S(INFO) << "Importing symbols...";

    DLOG_IF_S(ERROR,node.empty()) << "Symbols not found...";

    for (auto symbol : node.children("symbol"))
    {
        auto type = symbol.attribute("type").as_int();

        switch (type)
        {
            case Omap::SymbolTypeArea:
                {
                    ImportAreaSymbol(symbol);
                    break;
                }
            case Omap::SymbolTypeLine:
                {
                    ImportLineSymbol(symbol);
                    break;
                }
            case Omap::SymbolTypePoint:
                {
                    ImportPointSymbol(symbol);
                    break;
                }
            default:
                {

                break;
                }
        }


        LOG_S(INFO) << "Symbol with ID: " << symbol.attribute("code").as_int() * 1000;
    }
}



void OmapImporter::ImportParts(pugi::xml_node node)
{
    for (auto part : node.children("part"))
    {
        ImportObjects(part.child("objects"));
    }
}

void OmapImporter::ImportObjects(pugi::xml_node node)
{
    for (auto object : node.children("object"))
    {
        ImportObject(object);
    }
}

void OmapImporter::ImportColors(pugi::xml_node node)
{
    for (auto object : node.children("color"))
    {
    }
}


void OmapImporter::ImportObject(pugi::xml_node node)
{

    auto sym_idx = node.attribute("symbol").as_int();
    auto symbol = symbol_index_[sym_idx];

    std::unique_ptr<Object> object;

    if (!symbol)
    {
        LOG_S(WARNING) << "Object with symbol ID " << sym_idx << " has no associated symbol";
        return;
    }

    if (symbol->IsArea())
    {
        auto path_object = std::make_unique<PathObject>(symbol);
        FillPathCoords(path_object.get(),true,node.child("coords"));
        object = std::move(path_object);

        LOG_S(INFO) << "Imported area object with symbol " << object->symbol()->GetId();
    }
    else if (symbol->IsPath())
    {
        auto path_object = std::make_unique<PathObject>(symbol);
        FillPathCoords(path_object.get(),true,node.child("coords"));

        object = std::move(path_object);
        LOG_S(INFO) << "Imported path object with symbol " << object->symbol()->GetId();
    }
    else if (symbol->IsPoint())
    {
        auto point_object = std::make_unique<PointObject>(symbol);
        auto pos = ComputeObjectCoordinates(point_object.get(),node.child("coords"));
        point_object->SetPoint(pos[0].coordinate());
        object = std::move(point_object);

        LOG_S(INFO) << "Imported point object with symbol " << object->symbol()->GetId();
    }
    else
    {
        LOG_S(WARNING) << "Object of irrelevant type found";
        return;
    }

    map_->AppendObject(std::move(object));
}

void OmapImporter::ImportColor(pugi::xml_node node)
{
}

void OmapImporter::ImportLineSymbol(pugi::xml_node node)
{
    auto symbol = std::make_shared<Symbol>();
    auto sym_idx = node.attribute("id").as_int();

    if (SetupSymbol(node,symbol.get()))
    {
        symbol->type(PathS);
        LOG_S(INFO) << "Importing line symbol with ID " << symbol->GetId();

        symbol_index_.emplace(sym_idx,symbol);
        map_->AppendSymbol(std::move(symbol));
    }

}

void OmapImporter::ImportAreaSymbol(pugi::xml_node node)
{
    auto symbol = std::make_shared<Symbol>();
    auto sym_idx = node.attribute("id").as_int();

    if (SetupSymbol(node, symbol.get()))
    {
        symbol->type(AreaS);
        LOG_S(INFO) << "Importing area symbol with ID " << symbol->GetId();
        symbol_index_.emplace(sym_idx,symbol);
        map_->AppendSymbol(std::move(symbol));

    }
}

void OmapImporter::ImportPointSymbol(pugi::xml_node node)
{
    auto symbol = std::make_shared<Symbol>();
    auto sym_idx = node.attribute("id").as_int();

    if (SetupSymbol(node, symbol.get()))
    {
        symbol->type(PointS);
        LOG_S(INFO) << "Importing point symbol with ID " << symbol->GetId();
        symbol_index_.emplace(sym_idx,symbol);
        map_->AppendSymbol(std::move(symbol));

    }
}

bool OmapImporter::SetupSymbol(pugi::xml_node node, Symbol* symbol)
{

    assert(strcmp(node.name(), "symbol") == 0);


    size_t id = node.attribute("code").as_int() * 1000;
    symbol->SetId(id);

    auto it = std::ranges::find_if(attributes_,[id](const auto& attr)
    {
        return attr.id == id;
    });

    bool symbol_is_relevant= false;


    if (it != attributes_.end())
    {
        symbol->SetMapSymbol(it->flags & Irrelevant);
        symbol->SetCultivated(it->flags  & Cultivated);
        symbol->SetDirectional(it->flags & Directional);
        symbol->SetObstructing(it->flags & Obstructing);
        symbol->SetRadiusOfInfluence(it->radius_of_influence);
        symbol_is_relevant = true;
    }

    return symbol_is_relevant;
}

ObjectCoordinate OmapImporter::ConvertCoordinates(std::string coord_str)
{

    size_t previous_pos = 0;
    size_t current_pos = coord_str.find_first_of(' ');
    int32_t x, y;
    uint8_t flags = 0;
    if (current_pos == std::string::npos)
    {
        LOG_S(ERROR) << "Trouble with parsing of coordinate: Not enough components";
        throw std::runtime_error("Trouble with parsing of coordinate: Not enough components");
    }

    try
    {
        x = std::stoi(coord_str.substr(previous_pos, current_pos - previous_pos));

    } catch (std::invalid_argument& e)
    {
        LOG_S(ERROR) << "Trouble with parsing of coordinate: X coordinate badly formed\n" << e.what();
        throw;
    }

    previous_pos = current_pos + 1;
    current_pos = coord_str.find_first_of(' ', previous_pos);

    try
    {
        y = std::stoi(coord_str.substr(previous_pos, current_pos - previous_pos));
    } catch (std::invalid_argument& e)
    {
        LOG_S(ERROR) << "Trouble with parsing of coordinate: Y coordinate badly formed\n" << e.what();
        throw;
    }



    if (current_pos != std::string::npos)
    {
        previous_pos = current_pos + 1;
        current_pos = coord_str.find_first_of(' ', previous_pos);
        // Extra flags
        try
        {
            flags = std::stoi(coord_str.substr(previous_pos, current_pos - previous_pos));
        } catch (std::invalid_argument& e)
        {
            LOG_S(ERROR) << "Trouble with parsing of coordinate: flags badly formed\n" << e.what();
            throw;
        }
    }



    auto map_coord = Spatial2D({static_cast<float>(x), static_cast<float>(y)});

    auto proj_coord = map_coord / 100.f; //TODO: Georef
    ObjectCoordinate coord;
    coord.coordinate() = proj_coord;
    coord.flags() = flags;

    return coord;
}
std::vector<ObjectCoordinate> OmapImporter::ComputeObjectCoordinates(Object* object,  const pugi::xml_node object_node)
{
    std::vector<ObjectCoordinate> coords;

    assert(strcmp(object_node.name(), "coords") == 0);
    auto coordiantes_str = std::string(object_node.child_value());

    size_t previous_pos = 0;
    size_t current_pos = coordiantes_str.find_first_of(';');
    int number_of_coords = object_node.attribute("count").as_int();
    int coords_processed = 0;

    while (current_pos != std::string::npos)
    {
        auto substring = coordiantes_str.substr(previous_pos, current_pos - previous_pos);

        coords.push_back(ConvertCoordinates(substring));
        ++coords_processed;

        previous_pos = current_pos + 1;
        current_pos = coordiantes_str.find_first_of(';', previous_pos);
    }

    assert(coords_processed == number_of_coords);
    return coords;
}

void OmapImporter::FillPathCoords(PathObject* object, bool is_area, const pugi::xml_node object_node)
{
    auto coords = ComputeObjectCoordinates(object, object_node);
    object->BuildCurve(coords);
}

OmapImporter::~OmapImporter()
{
    for (auto item : symbol_index_)
    {
        item.second.reset();
    }
}
