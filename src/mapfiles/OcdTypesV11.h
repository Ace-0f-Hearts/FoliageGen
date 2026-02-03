//
// Created by ace on 2026-01-26.
//

#ifndef GENERATOR_OCDTYPESV11_H
#define GENERATOR_OCDTYPESV11_H
#include <cstdint>

#include "OcdTypes.h"
#include "OcdTypesV10.h"
#include "OcdTypesV9.h"

namespace Ocd
{
    class OcdTypesV11
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

        using IndexEntryType = uint32_t;
        using Icon = OcdTypesV9::Icon;
        using BasicTextAttributes = OcdTypesV9::BasicTextAttributes;
        using FramingAttributes = OcdTypesV9::FramingAttributes;
        using SpecialtextAttributes = OcdTypesV9::SpecialTextAttributes;
        using AreaSymbolGeneric = OcdTypesV9::AreaSymbolGeneric;
        using LineSymbolGeneric = OcdTypesV9::LineSymbolGeneric;


        struct BaseSymbol
        {
            static const int SYMBOL_NUMBER_FACTOR = 1000;
            using IndexEntryType = Ocd::SymbolIndexEntry;


            u32 size;
            u32 number;
            u8 type;
            u8 flags;
            u8 selected;
            u8 status;
            u8 tool;
            u8 cs_mode;
            u8 cs_type;
            u8 cd_flags;
            i32 extent;
            u32 file_pos;
            u8 RESERVED_MEMBER[2];
            i16 num_colors;
            u16 colors[14];
            OcdTypesGeneric::Utf16String<64> description;
            Icon icon;
            u16 group[64];
        };

        struct PointSymbol
        {
            using Element = OcdTypesV10::Format::PointSymbol::Element;

            BaseSymbol base;

            u16 dataSize;
            u16 RESERVED_MEMBER;
            Element* element;
        };

        struct LineSymbol
        {
            using Element = OcdTypesV10::Format::LineSymbol::Element;
            BaseSymbol base;

            LineSymbolGeneric generic;
            Element* element;
        };

        struct AreaSymbol
        {
            using Element = OcdTypesV10::Format::AreaSymbol::Element;

            BaseSymbol base;
            u32 borderSymbol;
            AreaSymbolGeneric generic;
            u16 RESERVED_MEMBER;
            u16 dataSize;
            Element* element;
        };

        struct TextSymbol
        {
            BaseSymbol base;

            OcdTypesGeneric::Utf8PascalString<31> fontName;
            BasicTextAttributes basic;
            SpecialtextAttributes special;
            u16 RESERVED_MEMBER;
            FramingAttributes framing;
        };

        struct LineTextSymbol
        {
            BaseSymbol base;

            OcdTypesGeneric::Utf8PascalString<31> fontName;
            BasicTextAttributes basic;
            FramingAttributes framing;
        };

        struct RectSymbol
        {
            BaseSymbol base;

            u16 line_color;
            u16 line_width;
            u16 corner_radius;
            u16 grid_flags;
            u16 cell_width;
            u16 cell_height;
            u16 RESERVED_MEMBER1[2];
            u16 unnumbered_cells;
            OcdTypesGeneric::Utf8PascalString<3> unnumbered_text;
            u16 line_style;
            OcdTypesGeneric::Utf8PascalString<31> RESERVED_MEMBER2;
            u16 RESERVED_MEMBER3;
            u16 font_size_V10; /// \since V10
            u16 RESERVED_MEMBER4[4];
        };

        struct Format
        {
            constexpr static u16 version = 11;
            using FileHeader = OcdTypesV10::Format::FileHeader;
            using BaseSymbol = BaseSymbol;
            using PointSymbol = PointSymbol;
            using LineSymbol = LineSymbol;
            using AreaSymbol = AreaSymbol;
            using TextSymbol = TextSymbol;
            using LineTextSymbol = LineTextSymbol;
            using RectangleSymbol = RectSymbol;
            using Object = OcdTypesV10::Format::Object;
            using Encoding = OcdTypesGeneric::Custom8BitEncoding;
        };
    };
}

#endif //GENERATOR_OCDTYPESV11_H
