//
// Created by ace on 2026-02-19.
//
#include <algorithm>
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
#include "ocad/ocad_types_v9.h"
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
        auto point_object = std::make_unique<PointObject>(symbol);
        const auto pos = ConvertOcadPoint(ocad_object.coords[0]);
        SetPointCoord(point_object.get(),pos);
        object = std::move(point_object);

        LOG_F(INFO,"Point object with symbol %d imported",ocad_object.symbol);
    }
    else
    {
        LOG_F(INFO,"Object with irrelevant type found");
        return;
    }


    map_->AppendObject(std::move(object));
}



void OcadImporter::SetPointCoord(PointObject* object, const OcadCoordinate& ocad_point)
{
    object->SetPoint(ocad_point.coordinate());
}

template <class F>
void OcadImporter::ImportSymbols(OcadFile<F>& file)
{
    for (auto symbol_idx : file.symbols())
    {
        auto& ocad_symbol = *symbol_idx.entity;

        if (symbol_idx.entity->status != Ocad::SymbolHidden)
        {
            switch (ocad_symbol.object_type)
            {
            case (SymbolTypePoint):
                {
                    ImportPointSymbol(reinterpret_cast<const typename F::PointSymbol&>(ocad_symbol));
                }
            case (SymbolTypeLine):
                {
                    ImportLineSymbol(reinterpret_cast<const typename F::LineSymbol&>(ocad_symbol));
                }
            case (SymbolTypeArea):
                {
                    ImportAreaSymbol(reinterpret_cast<const typename F::AreaSymbol&>(ocad_symbol));
                }
            default:
                {
                    // Irrelevant object
                    break;
                }
            }
        }

    }
}


template <class S>
void OcadImporter::ImportPointSymbol(const S& ocad_symbol)
{
    auto symbol = std::make_unique<Symbol>();

    if (SetupSymbol(symbol.get(), ocad_symbol))
    {
        symbol->type(PointS);

        auto color = ComputePointColor(ocad_symbol.data_size,ocad_symbol.begin_of_elements);
        if (color)
        {
            symbol->SetColor(color);
        }
        symbol_index_.emplace(symbol->GetId(),symbol.get());
        map_->AppendSymbol(std::move(symbol));
    }
}

template <class S>
void OcadImporter::ImportLineSymbol(const S& ocad_symbol)
{
    auto symbol = std::make_unique<Symbol>();



    if (SetupSymbol(symbol.get(), ocad_symbol))
    {
        symbol->type(PathS);
        auto color = ComputeLineColor(ocad_symbol,ocad_symbol.generic);
        if (color)
        {
            symbol->SetColor(color);
        }
        symbol_index_.emplace(symbol->GetId(),symbol.get());
        map_->AppendSymbol(std::move(symbol));
    }
}

template <class S>
void OcadImporter::ImportAreaSymbol(const S& ocad_symbol)
{
    auto symbol = std::make_unique<Symbol>();


    if (SetupSymbol(symbol.get(), ocad_symbol))
    {
        symbol->type(AreaS);

        LOG_S(INFO) << "Importing Area with ID " << symbol->GetId();
        auto color = ComputeAreaColor(ocad_symbol.generic.fill_on_V9,ocad_symbol.generic,ocad_symbol.data_size,ocad_symbol.begin_of_elements);
        if (color)
        {
            symbol->SetColor(color);
        }

        symbol_index_.emplace(symbol->GetId(),symbol.get());
        map_->AppendSymbol(std::move(symbol));
    }
}




template <class OcadBaseSymbol>
bool OcadImporter::SetupSymbol(Symbol* symbol, const OcadBaseSymbol& base)
{
    bool symbol_is_relevant = false;

    symbol->SetId(base.base.sym_num);
    auto id = symbol->GetId();

    auto it = std::ranges::find_if(attributes_.cbegin(),attributes_.cend(),[id](const auto& attr)
    {
        return attr.id == id;
    });
    if (it != attributes_.end())
    {
        symbol_is_relevant = true;
        symbol->SetMapSymbol(it->flags & Irrelevant);
        symbol->SetCultivated(it->flags  & Cultivated);
        symbol->SetDirectional(it->flags & Directional);
        symbol->SetObstructing(it->flags & Obstructing);
        symbol->SetRadiusOfInfluence(it->radius_of_influence);
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
    spot_colors_.clear();
    spot_colors_.reserve(10);

    HandleStrings(file, {{10, &OcadImporter::ImportSpotColor}});
    HandleStrings(file, {{9, &OcadImporter::ImportColor}});

    std::sort(spot_colors_.begin(), spot_colors_.end(), [](const auto a, const auto b) {
        return a->GetPriority() < b->GetPriority();
    });

    for (const auto& spot_color : spot_colors_)
    {
        map_->AppendColor(std::make_shared<MapColor>(*spot_color));
    }
}


void OcadImporter::ImportSpotColor(const std::string& param)
{
    OcadParameterStreamReader reader(param);

    const std::string name = reader.Value();
    int number = -1;
    Cmyk cmyk {0.f, 0.f, 0.f, 0.f};
    double screen_angle = 45;
    double screen_frequency = 150;

    while (reader.ReadNext())
    {
        float f_value;
        auto param_value = reader.Value();

        switch (reader.Key())
        {
            case 'n':
                {
                    number = std::stoi(param_value);
                    break;
                }
            case 'c':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0 & f_value <= 100)
                        cmyk.c = 0.01f * f_value;
                    break;
                }
            case 'm':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0 & f_value <= 100)
                        cmyk.m = 0.01f * f_value;
                    break;
                }
            case 'y':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0 & f_value <= 100)
                        cmyk.y = 0.01f * f_value;
                    break;
                }
            case 'k':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0 & f_value <= 100)
                        cmyk.k = 0.01f * f_value;
                    break;
                }
            case 'f':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0)
                    {
                        screen_frequency = 0.1 * f_value;
                    }
                    break;
                }
            case 'a':
                {
                    f_value = std::stof(param_value);
                    if (f_value >= 0)
                    {
                        screen_angle = f_value;
                    }
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
    std::shared_ptr<MapColor> color = std::make_shared<MapColor>();
    color->SetCmyk(cmyk);
    color->SetName(name);
    spot_colors_.push_back(std::make_shared<MapColor>(*color));
    color_index_.emplace(number,std::make_shared<MapColor>(*color));

}

void OcadImporter::ImportColor(const std::string& param)
{
    OcadParameterStreamReader parameters(param);

    const std::string name = parameters.Value();
    int number;
    bool number_ok;

    Cmyk cmyk{0,0,0,0};
    float opacity = 1.f;

    while (parameters.ReadNext())
    {
        float f_value;
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
        case 't':
            f_value = std::stof(param_value);
            if (f_value >= 0 && f_value <= 100)
                opacity = f_value;
            break;
        default:
            break;
        }
    }

    int color_prio = map_->GetColorsAmount();
    auto color = std::make_shared<MapColor>();
    color->SetCmyk(cmyk);
    color->SetPriority(color_prio);
    color->SetName(name);
    color->SetOpacity(opacity);
    map_->AppendColor(std::make_shared<MapColor>(*color));
    color_index_.emplace(number,std::make_shared<MapColor>(*color));

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
    return M_PI / 180.f * (0.1f * static_cast<float>((ocad_angle + 3600) % 3600));
}

MapColor* OcadImporter::ComputePointColor(std::size_t data_size, const OcadTypesV9::PointSymbolElement* elements)
{


    bool color_found = false;

    MapColor* computed_color = nullptr;

    for (size_t i = 0; i < data_size && !color_found; i+=2)
    {
        const auto* element = reinterpret_cast<const OcadTypesV9::PointSymbolElement*>(&reinterpret_cast<const Ocad::Generic::OcadCoord*>(elements)[i]);

        MapColor* inner_color = nullptr;
        MapColor* outer_color = nullptr;
        int outer_width = 0;
        int inner_radius = 0;

        switch (element->type)
        {
        case OcadTypesV9::PointSymbolElement::TypeDot:
            {
                if (element->diameter > 0)
                {
                    inner_color = ConvertColor(element->color);
                    inner_radius = ConvertLength(element->diameter) / 2;
                    outer_width = 0;
                }
                break;
            }
        case OcadTypesV9::PointSymbolElement::TypeCircle:
            {
                decltype(element->diameter) element_radius = (element->diameter - element->line_width) / 2;
                if (element_radius > 0 && element->line_width > 0)
                {
                    inner_radius = ConvertLength(element_radius);
                    outer_color = ConvertColor(element->color);
                    outer_width = ConvertLength(element->line_width);
                }
                break;

            }
        case OcadTypesV9::PointSymbolElement::TypeLine:
            {
                if (element->line_width > 0)
                {
                    computed_color = ConvertColor(element->color);
                }
                break;

            }
        case OcadTypesV9::PointSymbolElement::TypeArea:
            {

                computed_color = ConvertColor(element->color);
                break;
            }
        default:
            {
                LOG_S(WARNING) << "Type of element not supported";
            }
        }

        if (computed_color)
        {
            color_found = true;
        }
        else if (inner_color && outer_color)
        {
            if (inner_color->IsWhite())
            {
                computed_color = outer_color;
            }
            else if (outer_color->IsWhite())
            {
                computed_color = inner_color;
            }
            else
                computed_color = (std::powf(inner_radius,2) * M_PI > std::powf(inner_radius + outer_width, 2) * M_PI - std::powf(inner_radius,2) * M_PI) ? inner_color : outer_color;
            color_found = true;
        }
        else if (inner_color)
        {
            computed_color = inner_color;
            color_found = true;
        }
        else if (outer_color)
        {
            computed_color = outer_color;
            color_found = true;
        }

    }

    LOG_S(INFO) << "POINT COLOUR:" <<  computed_color;
    return computed_color;
}

template<class S>
MapColor* OcadImporter::ComputeLineColor(
    const S& ocad_symbol,const OcadTypesV9::LineSymbolGeneric attributes)
{
    MapColor* computed_color = nullptr;

    MapColor* main_color = nullptr;
    MapColor* border_color = nullptr;
    MapColor* right_border_color = nullptr;

    int main_line_width = 0.f;
    int border_width = 0.f;
    int right_border_width = 0.f;

    main_line_width = ConvertLength(attributes.line_width);
    main_color = main_line_width ? ConvertColor(attributes.line_color) : nullptr;


    auto const is_visible_double_line = [](auto const& ocad_attributes)
    {
        return ocad_attributes.double_mode != OcadTypesV9::LineSymbolGeneric::DoubleLineOff && (ocad_attributes.double_width > 0 || ocad_attributes.double_left_width > 0 || ocad_attributes.double_right_width > 0);
    };

    if (is_visible_double_line(ocad_symbol.generic))
    {
        if (main_line_width && attributes.double_flags & OcadTypesV9::LineSymbolGeneric::DoubleFlagFillColorOn)
            main_color = ConvertColor(attributes.double_color);

        border_width = ConvertLength(attributes.double_left_width);
        border_color = border_width ? ConvertColor(attributes.double_left_color) : nullptr;


        right_border_width = ConvertLength(attributes.double_right_width);
        right_border_color = right_border_width ? ConvertColor(attributes.double_right_color) : nullptr;
    }


    if (main_color)
    {
        if (border_color)
        {
            if (right_border_color)
            {
                if (main_line_width > 2 * border_width)
                    computed_color = main_line_width > 2 * right_border_width ? main_color : right_border_color;
                else
                    computed_color = border_width > right_border_width ? border_color : right_border_color;
            }
            else
            {
                computed_color = main_line_width > 2 * border_width ? main_color : border_color;
            }
        }
        else
        {
            if (right_border_color)
            {
                computed_color = main_line_width > 2 * right_border_width ? main_color : right_border_color;
            }
            else
                computed_color = main_color;
        }
    }


    // No color was able to be computed from line, left and right borders, trying line decorations
    if (!computed_color)
    {
        // TODO: SetupLineSymbolPointsSymbols

        const Ocad::Generic::OcadCoord* coords = reinterpret_cast<const Generic::OcadCoord*>(ocad_symbol.begin_of_elements);

        if (attributes.primary_data_size > 0)
        {
            // Mid symbol color
            computed_color = ComputePointColor(attributes.primary_data_size,ocad_symbol.begin_of_elements);
            return computed_color;
        }
        else if (attributes.secondary_data_size > 0)
        {
            // Mid symbol color
            computed_color = ComputePointColor(attributes.secondary_data_size,ocad_symbol.begin_of_elements);
            return computed_color;
        }
        if (attributes.start_data_size > 0)
        {
            // Start symbol color
            computed_color = ComputePointColor(attributes.start_data_size,reinterpret_cast<const OcadTypesV9::PointSymbolElement*>(coords));
            return computed_color;
        }
        if (attributes.end_data_size > 0)
        {
            // End symbol color
            computed_color = ComputePointColor(attributes.end_data_size,reinterpret_cast<const OcadTypesV9::PointSymbolElement*>(coords));
            return computed_color;
        }

        if (attributes.corner_data_size > 0)
        {
            // Dash symbol color
            computed_color = ComputePointColor(attributes.corner_data_size,reinterpret_cast<const OcadTypesV9::PointSymbolElement*>(coords));
            return computed_color;
        }


    }
    LOG_S(INFO) << "LINE COLOR: " << computed_color;
    return computed_color;
}

int OcadImporter::ConvertLength(int16_t ocad_length) const
{
    return ConvertLength<int16_t,int>(ocad_length);
}

int OcadImporter::ConvertLength(uint16_t ocad_length) const
{
    return ConvertLength<uint16_t,int>(ocad_length);
}

template <class T, class R>
R OcadImporter::ConvertLength(T ocad_length) const
{
    return static_cast<R>(ocad_length) * 10;
}

MapColor* OcadImporter::ComputeAreaColor(bool fill_on,
                                         const OcadTypesV9::AreaSymbolGeneric& ocad_symbol, std::size_t data_size,
                                         const OcadTypesV9::PointSymbolElement* elements)
{
    LOG_S(INFO) << "Fill on: " << fill_on << " : "<< ConvertColor(ocad_symbol.fill_color);
    auto computed_color = fill_on ? ConvertColor(ocad_symbol.fill_color) : nullptr;

    if (!computed_color && ocad_symbol.hatch_mode != Generic::HatchNone && ocad_symbol.hatch_line_width)
    {
        computed_color = ConvertLength(ocad_symbol.hatch_line_width) ?  ConvertColor(ocad_symbol.hatch_color) : nullptr;
    }

    if (!computed_color && ocad_symbol.structure_mode != Generic::StructureNone && ocad_symbol.structure_height && ocad_symbol.structure_width && data_size)
    {
        computed_color = ComputePointColor(data_size,elements);
    }

    return computed_color;
}

MapColor* OcadImporter::ConvertColor(int ocad_color)
{

    LOG_S(INFO) << "COLOR QUERY: " << ocad_color;

    if (!color_index_.contains(ocad_color))
    {
        LOG_F(WARNING,"Color %d not found, ignoring it.",ocad_color);
        return nullptr;
    }
    return color_index_[ocad_color].get();
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
