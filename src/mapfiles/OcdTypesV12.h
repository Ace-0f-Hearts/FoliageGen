//
// Created by ace on 2026-01-26.
//

#ifndef GENERATOR_OCDTYPESV12_H
#define GENERATOR_OCDTYPESV12_H
#include "OcdTypesV11.h"
#include "OcdTypesV11.h"


class OcdTypesV12
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

    struct FileHeader : OcdTypesV11::Format::FileHeader
    {
        u32 RESERVED_MEMBER[2];
        u32 firstMultiRepBlock;
    };

    using BaseSymbol = OcdTypesV11::BaseSymbol;
    using AreaSymbolGeneric = OcdTypesV11::AreaSymbolGeneric;

    struct AreaSymbol{
        using Element = OcdTypesV11::Format::PointSymbol::Element;

        BaseSymbol base;

        u32 border_symbol;
        AreaSymbolGeneric common;
        u8  structure_variation_x;
        u8  structure_variation_y;
        u16 structure_minimum_dist;
        u16 RESERVED_MEMBER;
        u16 data_size;

        Element begin_of_elements[1];
    };

    struct Object
    {
        using IndexEntryType = OcdTypesV11::Format::Object::IndexEntryType;

        i32  symbol;
        u8  type;
        u8  customer;
        i16  angle;
        i32  color;
        u16 line_width;
        u16 diam_flags;
        u32 server_object_id;
        u32 height;
        u64 creation_date;
        u32 multi_rep_id;
        u64 modification_date;
        u32 num_items;
        u16 num_text;
        u16 object_string_length;
        u16 db_link_length;
        u8  object_string_type;
        u8  RESERVED_MEMBER;

        OcdTypesGeneric::OcdCoord coords[1];
    };

    struct Format
    {
        using FileHeader = FileHeader;
        using BaseSymbol = BaseSymbol;
        using PointSymbol = OcdTypesV11::Format::PointSymbol;
        using LineSymbol = OcdTypesV11::Format::LineSymbol;
        using AreaSymbol = AreaSymbol;
        using TextSymbol = OcdTypesV11::Format::TextSymbol;
        using LineTextSymbol = OcdTypesV11::Format::LineTextSymbol;
        using RectSymbol = OcdTypesV11::Format::RectangleSymbol;
        using Object = Object;
        using Encoding = OcdTypesGeneric::Custom8BitEncoding;
    };

};


#endif //GENERATOR_OCDTYPESV12_H