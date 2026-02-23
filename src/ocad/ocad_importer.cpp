//
// Created by ace on 2026-02-19.
//
#include <loguru.hpp>
#include <ocad/ocad_importer.h>


#include "../../include/spatial/orienteering_path.h"
#include "ocad/ocad_helper.h"
#include "ocad/ocad_types_v10.h"
#include "ocad/ocad_types_v11.h"
#include "ocad/ocad_types_v12.h"
#include "ocad/ocad_types_v9.h"
#include "orienteering/object.h"
#include "orienteering/path_object.h"

using namespace Ocad;

Ocad::OcadImporter::OcadImporter(const std::filesystem::path& path, Map map) : Importer(path, map), buffer_(kBuffer_size)
{
}

bool Ocad::OcadImporter::ImportImplementation()
{
    input_stream_.readsome(buffer_.data(),kBuffer_size);

    if (buffer_.empty())
    {
        LOG_F(ERROR,"File buffer is empty!");
        throw std::invalid_argument("File buffer is empty!");
    }

    if (buffer_.size() < sizeof(Generic::FileHeaderGeneric))
    {
        LOG_F(ERROR,"File buffer does not contain a header!");
        throw std::invalid_argument("File buffer does not contain a header!");
    }

    auto header = reinterpret_cast<const Generic::FileHeaderGeneric*>(buffer_.data());

    if (header->version != 0x0cad)
    {
        LOG_F(ERROR,"Invalid data in header.");
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
        default:
        throw std::invalid_argument("Invalid format");

    }
    return true;
}

template <class F>
void Ocad::OcadImporter::ImportImplementation()
{
    OcadFile<F> file(buffer_);

    if (!file.header())
        LOG_F(ERROR,"Incomplete or missing header!");

    ImportSymbols(file);
    ImportObjects(file);

}

template <class F>
void Ocad::OcadImporter::ImportObjects(OcadFile<F>& file)
{
    for (auto object : file.objects())
    {
        if (object.entry->symbol)
            ImportObject(*object.entity);
    }
}

template <class O>
void OcadImporter::ImportObject(const O& object)
{
}

template <class F>
void OcadImporter::ImportSymbols(OcadFile<F>& file)
{
    for (auto object : file.symbols())
    {
        ImportSymbol(*object.entity);
    }
}

template<class S>
void OcadImporter::ImportSymbol(const S& base)
{
    auto symbol = new Orienteering::Symbol();
    SetupSymbol(symbol,base);
}


template <class OcadBaseSymbol>
void OcadImporter::SetupSymbol(Orienteering::Symbol* symbol, const OcadBaseSymbol& base)
{

}

void OcadImporter::FillPathCoords(PathObject *object, bool is_area, uint32_t num_points, const Generic::OcadCoord* ocd_points)
{
    std::vector<OcadCoordinate> path;
    path.reserve(num_points);
    object->coordinates().reserve(num_points);

    for (auto i = 0u; i < num_points; i++)
    {
        path[i] = ConvertOcadPoint(ocd_points[i]);
        SetPointFlags(object, i, is_area, ocd_points[i]);
    }

    // For path objects, create closed parts where the position of the last point is equal to that of the first point
    if (object->type() == ObjectType::Path)
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
                path[i-2].SetCurveStart(false);
            case 1:
                path[i-1].SetCurveStart(false);
            case 0:
                ; // nothing
            }

            start = i + 1;
        }
    }


}
