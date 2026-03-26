//
// Created by ace on 2026-02-19.
//
#include <iostream>
#include <string.h>
#include <ocad/ocad_importer.h>

#include <loguru.hpp>

#include <ocad/ocad_helper.h>
#include <ocad/ocad_types_v10.h>
#include <ocad/ocad_types_v11.h>
#include <ocad/ocad_types_v12.h>
#include <ocad/ocad_types_v9.h>

#include <orienteering/object.h>
#include <orienteering/path_object.h>

#include "ocad/ocad_georef.h"
#include "ocad/ocad_paramater_stream_reader.h"
#include "ocad/ocad_types_v2018.h"
#include "orienteering/georeferencing.h"
#include "orienteering/map_color.h"
#include "utility/not_implemented_error.h"

using namespace Ocad;

OcadImporter::OcadImporter(const std::filesystem::path& path, std::shared_ptr<OrienteeringMap> map,const std::vector<SymbolAttribute>& attributes) :
    Importer(path, map,attributes),
    buffer_(kBuffer_size), georef_(), ocad_version_(0)
{
    input_stream_.open(path_.string(), std::ios::in | std::ios::binary);
    if (!input_stream_.good())
    {
        std::cerr << "Failed to open file: " << input_stream_.rdstate() << std::endl;
        throw std::invalid_argument("Cannot open file!");
    }
}

void OcadImporter::ReadFile()
{
    auto file_size = std::filesystem::file_size(path_);

    buffer_.resize(file_size);
    input_stream_.readsome(reinterpret_cast<char*>(buffer_.data()),file_size);
}


bool OcadImporter::ImportImplementation()
{
    ReadFile();


    if (buffer_.empty())
    {
        LOG_F(ERROR, "File buffer is empty!");
        throw std::invalid_argument("File buffer is empty!");
    }

    if (buffer_.size() < sizeof(Generic::FileHeaderGeneric))
    {
        LOG_F(ERROR, "File buffer does not contain a header!");
        throw std::invalid_argument("File buffer does not contain a header!");
    }

    auto header = reinterpret_cast<const Generic::FileHeaderGeneric*>(buffer_.data());

    if (header->vendor_mark != 0x0cad)
    {
        LOG_F(ERROR, "Invalid data in header: %d.",header->vendor_mark);
        throw std::invalid_argument("Invalid data");
    }

    switch (header->version)
    {
    case 9:
        ImportImplementation<OcadTypesV9::Format>();
        break;
    case 10:
        ImportImplementation<OcadTypesV10::Format>();
        break;
    case 11:
        ImportImplementation<OcadTypesV11::Format>();
        break;
    case 12:
        ImportImplementation<OcadTypesV12::Format>();
        break;
    case 2018:
        LOG_F(WARNING,"Support OCAD version %d files is experimental.",header->version);
        static_assert(std::is_same_v<OcadTypesV12::Format,OcadTypesV2018::Format>);
        ImportImplementation<OcadTypesV12::Format>();
        break;
    default:
        throw std::invalid_argument("Invalid format");
    }
    return true;
}

template <class F>
void OcadImporter::ImportImplementation()
{
    OcadFile<F> file(buffer_);

    if (!file.header())
        LOG_F(ERROR, "Incomplete or missing header!");

    ImportGeoreferencing(file);
    ImportColors(file);
    ImportSymbols(file);
    ImportObjects(file);
}

template <class F>
void OcadImporter::ImportObjects(OcadFile<F>& file)
{
    for (auto ocad_object : file.objects())
    {
        if (ocad_object.entry->status != Ocad::ObjectDeleted && ocad_object.entry->status != Ocad::ObjectDeletedForUndo)
            ImportObject(*ocad_object.entity);
    }
}

template <class O>
void OcadImporter::ImportObject(const O& ocad_object)
{
    Symbol* symbol = symbol_index_[ocad_object.symbol];
    std::unique_ptr<Object> object;

    if (!symbol)
    {
        LOG_F(INFO,"Object's symbol not found in index");
        return;
    }

    if (symbol->IsArea())
    {
        auto path_object = std::make_unique<PathObject>(symbol);
        //
        FillPathCoords(path_object.get(),true,ocad_object.num_items,reinterpret_cast<const Generic::OcadCoord *>(ocad_object.coords));
        object = std::move(path_object);
        LOG_F(INFO,"Area object with symbol %d imported",ocad_object.symbol);
    }
    else if (symbol->IsPath())
    {
        auto path_object = std::make_unique<PathObject>(symbol);
        FillPathCoords(path_object.get(),false,ocad_object.num_items,reinterpret_cast<const Generic::OcadCoord *>(ocad_object.coords));
        object = std::move(path_object);
        LOG_F(INFO,"Path object with symbol %d imported",ocad_object.symbol);
    }
    else if (symbol->IsPoint())
    {
        object = std::make_unique<PointObject>(symbol);
        LOG_F(INFO,"Point object with symbol %d imported",ocad_object.symbol);
    }
    else
    {
        LOG_F(INFO,"Object with irrelevant type found");
        return;
    }


    map_->AppendObject(std::move(object));
}

template <class F>
void OcadImporter::ImportSymbols(OcadFile<F>& file)
{
    for (auto symbol : file.symbols())
    {



        if (symbol.entity->status != Ocad::SymbolHidden)
            ImportSymbol(*symbol.entity);
    }
}

template <class S>
void OcadImporter::ImportSymbol(const S& base)
{


    auto symbol = std::make_unique<Symbol>();
    if (SetupSymbol(symbol.get(), base))
    {
        symbol_index_.emplace(symbol->id(),symbol.get());
        map_->AppendSymbol(std::move(symbol));
    }

}


template <class OcadBaseSymbol>
bool OcadImporter::SetupSymbol(Symbol* symbol, const OcadBaseSymbol& base)
{
    symbol->id(base.sym_num);
    auto symbol_is_relevant = false;

    auto id = symbol->id();

    auto idx = std::ranges::find_if(attributes_.cbegin(),attributes_.cend(),[id](const auto& attr)
    {
        LOG_S(INFO) << attr.id << " -> " << id;
        return attr.id == id;
    });
    if (idx != attributes_.end())
    {
        symbol_is_relevant = true;
        symbol->SetCultivated(idx->flags  & Cultivated);
        symbol->SetDirectional(idx->flags & Directional);
        symbol->SetObstructing(idx->flags & Obstructing);
    }


    if (symbol_is_relevant && base.object_type == SymbolTypePoint)
    {
        symbol->type(PointS);
        symbol_is_relevant = true;
    }
    else if (symbol_is_relevant && base.object_type == SymbolTypeLine)
    {
        symbol->type(PathS);
        symbol_is_relevant = true;
    }
    else if (symbol_is_relevant && base.object_type == SymbolTypeArea)
    {
        symbol->type(AreaS);
        symbol_is_relevant = true;
    }

    return symbol_is_relevant;
}

void OcadImporter::FillPathCoords(PathObject* object, bool is_area, uint32_t num_points,
                                  const Generic::OcadCoord* ocad_points)
{
    std::vector<OcadCoordinate> path;
    path.resize(num_points);

    for (auto i = 0u; i < num_points; i++)
    {
        path[i] = ConvertOcadPoint(ocad_points[i]);
        SetPointFlags(path, i, is_area, ocad_points[i]);
    }

    // For path objects, create closed parts where the position of the last point is equal to that of the first point
    if (object->type() == PathO)
    {
        size_t start = 0;
        for (size_t i = 0; i < path.size(); ++i)
        {
            if (!path[i].IsHolePoint() && i < path.size() - 1)
                continue;

            auto coord = path[start];

            coord.SetHolePoint(path[i].IsHolePoint());
            coord.SetClosePoint(true);
            coord.SetCurveStart(false);
            if (path[i].PositionEqualTo(coord))
            {
                // This segment has the canonical closed form: The coordinates
                // of the last point are identical to the first point.
                path[i].SetFlags(coord.flags());
                assert(path.front().PositionEqualTo(path.back()));
            }
            else if (is_area)
            {
                // We need to turn the segment into the canonical closed form
                // by inserting an extra end point.
                using difference_type = decltype(path)::difference_type;
                auto const after_i = std::ranges::cbegin(path) + i + 1;
                path.insert(after_i, coord);
                path[i].SetHolePoint(false);
                ++i;

                assert(path.front().PositionEqualTo(path.back()));
            }

            switch (i - start)
            {
            default:
                path[i - 2].SetCurveStart(false);
            case 1:
                path[i - 1].SetCurveStart(false);
            case 0:
                ; // nothing
            }

            start = i + 1;
        }
    }

    object->BuildCurve(path);
}

void OcadImporter::SetPointFlags(std::vector<OcadCoordinate>& object, uint32_t pos, bool is_area,
                                 Generic::OcadCoord ocd_point)
{
    if (ocd_point.IsFirstCurvePoint() && pos > 0)
    {
        object[pos - 1].SetCurveStart(true);
    }
    if (ocd_point.IsDashPoint() || ocd_point.IsCornerPoint())
    {
        object[pos].SetDashPoint(true);
    }
    if (ocd_point.IsFirstHolePoint() && pos > 1 && is_area)
    {
        object[pos - 1].SetHolePoint(true);
    }
}

OcadCoordinate OcadImporter::ConvertOcadPoint(const Generic::OcadCoord& ocad_point)
{
    OcadCoordinate result;

    int32_t ocad_x = ocad_point.x >> 8;
    int32_t ocad_y = ocad_point.y >> 8;

    constexpr auto invalid_value = int32_t(0x80000000u) >> 8; // ... so we use this value here.
    if (ocad_x == invalid_value)
        ocad_x = 0;
    if (ocad_y == invalid_value)
        ocad_y = 0;
    uint8_t flags = 0;
    if (ocad_point.IsFirstCurvePoint())
    {
        flags |= OcadCoordinate::CurveStart;
    }
    if (ocad_point.IsFirstHolePoint())
    {
        flags |= OcadCoordinate::HolePoint;
    }
    if (ocad_point.IsDashPoint())
    {
        flags |= OcadCoordinate::DashPoint;
    }

    auto map_coord = Spatial2D({static_cast<float>(ocad_x), static_cast<float>(ocad_y)});
    auto proj_coord = georef_.ToProjectedCoords(map_coord) / 100.;


    result.coordinate() = proj_coord;
    result.flags() = flags;
    return result;
}

template <class F>
void OcadImporter::ImportColors(const OcadFile<F>& file)
{
    HandleStrings(file, {{9, &OcadImporter::ImportColor}});
}

void OcadImporter::ImportColor(const std::string& param)
{
    OcadParameterStreamReader parameters(param);

    const std::string name = parameters.Value();
    int number;
    bool number_ok;

    Cmyk cmyk{0,0,0,0};
    bool overprinting = false;
    float opacity = 1.f;
    std::string spot_color_name;

    while (parameters.ReadNext())
    {
        float f_value;
        int i_value;
        auto param_value = parameters.Value();

        switch (parameters.Key())
        {
        case 'n':
            number = std::stoi(param_value);
            break;
        case 'c':
            f_value = std::stof(param_value);
            if (f_value >= 0 & f_value <= 100.f)
                cmyk.c = f_value;
            break;
        case 'm':
            f_value = std::stof(param_value);
            if (f_value >= 0 & f_value <= 100.f)
                cmyk.m = f_value;
            break;
        case 'y':
            f_value = std::stof(param_value);
            if (f_value >= 0 && f_value <= 100.f)
                cmyk.y = f_value;
            break;
        case 'k':
            f_value = std::stof(param_value);
            if (f_value >= 0 && f_value <= 100)
                cmyk.k = f_value;
            break;
        case 'o':
            overprinting = std::stoi(param_value);
            break;
        case 't':
            f_value = std::stof(param_value);
            if (f_value >= 0 && f_value <= 100)
                opacity = f_value;
            break;
        case 's':
            spot_color_name = param_value;
            break;
        case 'p':

            break;
        default:
            break;
        }
    }
    MapColor color;

    int color_prio = map_->GetColorsAmount();

    color.cmyk = cmyk;
    color.priority = color_prio;
    color.opacity = opacity;
    map_->AppendColor(cmyk);
}

template <class F>
void OcadImporter::ImportGeoreferencing(const OcadFile<F>& file)
{
    std::initializer_list<StringHandler> string_handlers = {{1039,&OcadImporter::ImportGeoreferencing}};
    HandleStrings(file, string_handlers);
}

void OcadImporter::ImportGeoreferencing(const std::string& param)
{
    OcadParameterStreamReader parameters(param);

    OcadGeoref ocad_georef;

    while (parameters.ReadNext())
    {
        auto param_value = parameters.Value();

        switch (parameters.Key())
        {
        case 'm':
            TryParamConvert(ocad_georef.m,param_value);
            break;
        case 'x':
            TryParamConvert(ocad_georef.x,param_value);
            break;
        case 'y':
            TryParamConvert(ocad_georef.y,param_value);
            break;
        case 'i':
            TryParamConvert(ocad_georef.i,param_value);
            break;
        case 'r':
            TryParamConvert(ocad_georef.r,param_value);
            break;
        case 'a':
            {
                ocad_georef.a = stod(param_value);
            }
            break;
        case 'd':
            {
                auto spacing = stod(param_value);
                if (spacing >= 0.001)
                {

                }
            }
            break;
        default:
            ;
        }
    }

    LOG_S(INFO) << ocad_georef;
    ocad_georef.SetupGeoref(georef_);

    georef_.UpdateTransformation();
}

float OcadImporter::ConvertOcadAngle(int ocad_angle)
{
    throw NotImplementedError();
}


template <class F>
void OcadImporter::HandleStrings(const OcadFile<F>& file, std::initializer_list<StringHandler> handlers)
{
    for (const auto ocad_string : file.strings())
    {
        for (const auto& handler : handlers)
        {
            if (ocad_string.entry->type == handler.type)
            {
                auto param_string = std::string(reinterpret_cast<const char*>(ocad_string.entity),ocad_string.entry->size);
                (this->*handler.callback)(param_string);
            }
        }
    }
}
