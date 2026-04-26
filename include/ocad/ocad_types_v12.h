//
// Created by ace on 2026-01-26.
//

#ifndef OCAD_OCDTYPESV12_H
#define OCAD_OCDTYPESV12_H
#include "ocad_types_v11.h"

namespace Ocad
{
    class OcadTypesV12
    {
    public:
        using u64 = uint64_t;
        using u32 = uint32_t;
        using u16 = uint16_t;
        using u8 = uint8_t;

        using i64 = int64_t;
        using i32 = int32_t;
        using i16 = int16_t;
        using i8 = int8_t;

#pragma pack(push, 1)
        struct FileHeader : OcadTypesV11::Format::FileHeader
        {
            u32 RESERVED_MEMBER[2]{};
            u32 firstMultiRepBlock{};
        };

        using BaseSymbol = OcadTypesV11::BaseSymbol;
        using AreaSymbolGeneric = OcadTypesV11::AreaSymbolGeneric;

        struct AreaSymbol
        {
            using Element = OcadTypesV11::Format::PointSymbol::Element;

            BaseSymbol base;

            u32 border_symbol;
            AreaSymbolGeneric generic;
            u8 structure_variation_x;
            u8 structure_variation_y;
            u16 structure_minimum_dist;
            u16 RESERVED_MEMBER;
            u16 data_size;

            Element begin_of_elements[1];
        };

        struct Object
        {
            using IndexEntryType = OcadTypesV11::Format::Object::IndexEntryType;

            i32 symbol;
            u8 type;
            u8 customer;
            i16 angle;
            i32 color; // Graphics relevant
            u16 line_width; // Graphics relevant
            u16 diam_flags; // Graphics relevant
            u32 server_object_id;
            u32 height;
            u64 creation_date;
            u32 multi_rep_id;
            u64 modification_date;
            u32 num_items;
            u16 num_text;
            u16 object_string_length;
            u16 db_link_length;
            u8 object_string_type;
            u8 RESERVED_MEMBER;

            Generic::OcadCoord coords[1];
        };
#pragma pack(pop)
        struct Format
        {
            using FileHeader = FileHeader;
            using BaseSymbol = BaseSymbol;
            using PointSymbol = OcadTypesV11::Format::PointSymbol;
            using LineSymbol = OcadTypesV11::Format::LineSymbol;
            using AreaSymbol = AreaSymbol;
            using TextSymbol = OcadTypesV11::Format::TextSymbol;
            using LineTextSymbol = OcadTypesV11::Format::LineTextSymbol;
            using RectSymbol = OcadTypesV11::Format::RectangleSymbol;
            using Object = Object;
            using Encoding = Generic::Utf8Encoding;
        };
    };
}

inline std::ostream& operator<<(std::ostream& os, Ocad::OcadTypesV12::Object object)
{
    os
    << "(" << object.symbol
    << ";" << object.type
    << ";" << object.customer
    << ";" << object.angle
    << ";" << object.color
    << ";" << object.line_width
    << ";" << object.diam_flags
    << ";" << object.server_object_id
    << ";" << object.height
    << ";" << object.creation_date
    << ";" << object.multi_rep_id
    << ";" << object.modification_date
    << ";" << object.num_items
    << ";" << object.num_text
    << ";" << object.object_string_length
    << ";" << object.db_link_length
    << ";" << object.object_string_type

    << ")"
    ;
    return os;
}



#endif //OCAD_OCDTYPESV12_H
