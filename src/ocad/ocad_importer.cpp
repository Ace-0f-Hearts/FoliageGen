//
// Created by ace on 2026-02-19.
//
#include <iostream>
#include <ocad/ocad_importer.h>

#include <loguru.hpp>

#include <ocad/ocad_helper.h>
#include <ocad/ocad_types_v10.h>
#include <ocad/ocad_types_v11.h>
#include <ocad/ocad_types_v12.h>
#include <ocad/ocad_types_v9.h>

#include <orienteering/object.h>
#include <orienteering/path_object.h>

#include "ocad/ocad_types_v2018.h"

using namespace Ocad;

OcadImporter::OcadImporter(const std::filesystem::path& path, std::shared_ptr<Map> map) : Importer(path, map),
    buffer_(kBuffer_size), ocad_version_(0)
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

    ImportSymbols(file);
    ImportObjects(file);
}

template <class F>
void OcadImporter::ImportObjects(OcadFile<F>& file)
{
    for (auto object : file.objects())
    {
        std::cout << *object.entity << std::endl;
        ImportObject(*object.entity);

    }
}

template <class O>
void OcadImporter::ImportObject(const O& ocad_object)
{
    Symbol* symbol = symbol_index_[ocad_object.symbol];
    std::unique_ptr<Object> object;

    if (!symbol)
        return;

    if (symbol->IsArea())
    {
        auto path_object = std::make_unique<PathObject>(symbol);
        //
        LOG_F(INFO,"Area object with symbol %d imported",ocad_object.symbol);
        FillPathCoords(path_object.get(),true,ocad_object.num_items,reinterpret_cast<const Generic::OcadCoord *>(ocad_object.coords));

    }

    if (symbol->IsPath())
    {
        // std::cout << ocad_object.num_items << std::endl;
        auto path_object = std::make_unique<PathObject>(symbol);
        FillPathCoords(path_object.get(),false,ocad_object.num_items,reinterpret_cast<const Generic::OcadCoord *>(ocad_object.coords));
        object = std::move(path_object);
        LOG_F(INFO,"Path object with symbol %d imported",ocad_object.symbol);
    }
    if (symbol->IsPoint())
    {
        object = std::make_unique<PointObject>(symbol);
        LOG_F(INFO,"Point object with symbol %d imported",ocad_object.symbol);
    }
    map_->AppendObject(std::move(object));
}

template <class F>
void OcadImporter::ImportSymbols(OcadFile<F>& file)
{
    for (auto object : file.symbols())
    {
        ImportSymbol(*object.entity);
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
    if (base.status & SymbolHidden)
        return false;
    symbol->id(base.sym_num);

    if (base.object_type & SymbolTypePoint)
    {
        symbol->SetPoint(true);
    }
    else if (base.object_type & SymbolTypeLine)
    {
        symbol->SetPath(true);
    }
    else if (base.object_type & SymbolTypeArea)
    {
        symbol->SetArea(true);
    }

    return true;
}

void OcadImporter::FillPathCoords(PathObject* object, bool is_area, uint32_t num_points,
                                  const Generic::OcadCoord* ocad_points)
{
    std::vector<OcadCoordinate> path;
    path.resize(num_points);
    object->coordinates().reserve(num_points);

    for (auto i = 0u; i < num_points; i++)
    {
        path[i] = ConvertOcadPoint(ocad_points[i]);
        SetPointFlags(path, i, is_area, ocad_points[i]);
    }

    // For path objects, create closed parts where the position of the last point is equal to that of the first point
    if (object->type() == PathO)
    {
        size_t start = 0;
        for (size_t i = 0; i < object->coordinates().size(); ++i)
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

    float x = ocad_point.x;
    float y = ocad_point.y;

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

    result.coordinate() = Spatial2D({x, y});
    result.flags() = flags;
    return result;
}
