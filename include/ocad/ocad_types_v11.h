//
// Created by ace on 2026-01-26.
//

#ifndef OCAD_OCDTYPESV11_H
#define OCAD_OCDTYPESV11_H
#include <cstdint>

#include "ocad_types.h"
#include "ocad_types_v10.h"
#include "ocad_types_v9.h"

namespace Ocad
{
    class OcadTypesV11
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
        using Icon = OcadTypesV9::Icon;
        using BasicTextAttributes = OcadTypesV9::BasicTextAttributes;
        using FramingAttributes = OcadTypesV9::FramingAttributes;
        using SpecialtextAttributes = OcadTypesV9::SpecialTextAttributes;
        using AreaSymbolGeneric = OcadTypesV9::AreaSymbolGeneric;
        using LineSymbolGeneric = OcadTypesV9::LineSymbolGeneric;

#pragma pack(push,1)

        struct BaseSymbol
        {
            static constexpr int kSymbolNumberFactor = 1000;
            using IndexEntryType = SymbolIndexEntry;


            u32 size;
            u32 sym_num;
            u8 object_type;
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
            Generic::Utf16String<64> description;
            Icon icon;
            u16 group[64];
        };

        struct PointSymbol
        {
            using Element = OcadTypesV10::Format::PointSymbol::Element;

            BaseSymbol base;

            u16 data_size;
            u16 RESERVED_MEMBER;
            Element begin_of_elements[1];
        };

        struct LineSymbol
        {
            using Element = OcadTypesV10::Format::LineSymbol::Element;
            BaseSymbol base;

            LineSymbolGeneric generic;
            Element begin_of_elements[1];
        };

        struct AreaSymbol
        {
            using Element = OcadTypesV10::Format::AreaSymbol::Element;

            BaseSymbol base;
            u32 border_symbol;
            AreaSymbolGeneric generic;
            u16 RESERVED_MEMBER;
            u16 data_size;
            Element begin_of_elements[1];
        };

        struct TextSymbol
        {
            BaseSymbol base;

            Generic::Utf8PascalString<31> font_name;
            BasicTextAttributes basic;
            SpecialtextAttributes special;
            u16 RESERVED_MEMBER;
            FramingAttributes framing;
        };

        struct LineTextSymbol
        {
            BaseSymbol base;

            Generic::Utf8PascalString<31> font_name;
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
            Generic::Utf8PascalString<3> unnumbered_text;
            u16 line_style;
            Generic::Utf8PascalString<31> RESERVED_MEMBER2;
            u16 RESERVED_MEMBER3;
            u16 font_size; /// \since V10
            u16 RESERVED_MEMBER4[4];
        };

#pragma pack(pop)

        struct Format
        {
            constexpr static u16 version = 11;
            using FileHeader = OcadTypesV10::Format::FileHeader;
            using BaseSymbol = BaseSymbol;
            using PointSymbol = PointSymbol;
            using LineSymbol = LineSymbol;
            using AreaSymbol = AreaSymbol;
            using TextSymbol = TextSymbol;
            using LineTextSymbol = LineTextSymbol;
            using RectangleSymbol = RectSymbol;
            using Object = OcadTypesV10::Format::Object;
            using Encoding = Generic::Utf8Encoding;
        };
    };
}

inline std::ostream& operator<<(std::ostream& os,Ocad::OcadTypesV11::BaseSymbol base)
{
    os
    << "(" << base.size
    << ";" << base.sym_num
    << ";" << base.object_type
    << ";" << base.flags
    << ";" << base.selected
    << ";" << base.status
    << ";" << base.cs_mode
    << ";" << base.cs_type
    << ";" << base.cd_flags
    << ";" << base.extent
    << ";" << base.file_pos
    << ";" << base.group
    << ";" << base.num_colors
    << ")";
    return os;
}
#endif //OCAD_OCDTYPESV11_H
