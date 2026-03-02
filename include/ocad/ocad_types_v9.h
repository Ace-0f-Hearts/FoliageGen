//
// Created by ace on 2026-01-26.
//

#ifndef OCAD_OCDTYPESV9_H
#define OCAD_OCDTYPESV9_H
#include <cstddef>
#include <cstdint>
#include <sys/types.h>

#include "ocad_helper.h"


namespace Ocad
{
    class OcadTypesV9
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

        struct BasicTextAttributes
        {
            u16 color;
            u16 font_size;
            u16 font_weight;
            u8 font_italic;
            u8 RESERVED;
            u16 char_spacing;
            u16 word_spacing;
            u16 alignment;
        };

        struct SpecialTextAttributes
        {
            u16 line_spacing;
            i16 para_spacing;
            u16 indent_first_line;
            u16 indent_other_lines;
            u16 num_tabs;
            i32 tab_pos[32];
            u16 line_below_on;
            u16 line_below_color;
            u16 line_below_width;
            u16 line_below_offset;
        };

        struct FramingAttributes
        {
            u8 mode; /// 16 bit in V8
            u8 line_style_V9; /// Since V9
            u8 point_symbol_on_V10; /// Since V10
            u32 point_symbol_number_V10; /// Since V10
            char RESERVED_MEMBER[19];
            u16 border_left_V9; /// Since V9; TextSymbol only
            u16 border_bottom_V9; /// Since V9; TextSymbol only
            u16 border_right_V9; /// Since V9; TextSymbol only
            u16 border_top_V9; /// Since V9; TextSymbol only
            u16 color;
            u16 line_width;
            u16 font_weight; /// TextSymbol only
            u16 italic; /// TextSymbol only
            i16 offset_x;
            i16 offset_y;
        };

        struct FileHeader : public Ocad::Generic::FileHeaderGeneric
        {
            u32 firstSymbolBlock;
            u32 firstObjectBlock;
            u32 offlineSyncSerial; // Since V11
            u32 currentFileVersion; // Since V12
            u32 RESERVED_MEMBER1;
            u32 RESERVED_MEMBER2;
            u32 firstStringBlock;
            u32 fileNamePosition;
            u32 fileNameSize;
            u32 RESERVED_MEMBER3;
        };

        struct Icon
        {
            u8 bits[484];
        };


        struct BaseSymbol
        {
            using IndexEntryType = SymbolIndexEntry;
            static constexpr int kSymbolNumberFactor = 1000;

            u32 size;
            u32 sym_num;
            u8 objectType;
            u8 flags;
            u8 selected;
            u8 status;
            u8 drawingTool;
            u8 csMod;
            u8 csObjType;
            u8 csCdFlags;
            u32 extent;
            u32 filePos;
            u16 group;
            u16 numOfColors;
            u16 colors[14];
            Ocad::Generic::PascalString<31> name;
            Icon icon;
        };

        struct PointSymbolElement
        {
            u16 type;
            u16 flags;
            u16 color;
            u16 lineWidth;
            u16 diameter;
            u16 numOfCoords;
            u16 RESERVED1;
            u16 RESERVED2;
        };

        struct PointSymbol : BaseSymbol
        {
            using Element = PointSymbolElement;

            BaseSymbol base;

            u16 dataSize;
            u16 RESERVED;
            Element* element;
        };


        struct LineSymbolGeneric
        {
            u16 line_color;
            u16 line_width;
            u16 line_style;
            i16 dist_from_start;
            i16 dist_from_end;
            i16 main_length;
            i16 end_length;
            i16 main_gap;
            i16 sec_gap;
            i16 end_gap;
            i16 min_sym;
            i16 num_prim_sym;
            i16 prim_sym_dist;
            u16 double_mode;
            u16 double_flags;
            u16 double_color;
            u16 double_left_color;
            u16 double_right_color;
            i16 double_width;
            i16 double_left_width;
            i16 double_right_width;
            i16 double_length;
            i16 double_gap;
            u16 double_background_color_V11; /// Since V11
            u16 RESERVED_MEMBER1;
            u16 RESERVED_MEMBER2;
            u16 dec_mode;
            u16 dec_last;
            u16 RESERVED_MEMBER3;
            u16 framing_color;
            i16 framing_width;
            u16 framing_style;
            u16 primary_data_size;
            u16 secondary_data_size;
            u16 corner_data_size;
            u16 start_data_size;
            u16 end_data_size;
            u8 active_symbols_V11; /// Since V11
            u8 RESERVED_MEMBER4;

            enum LineStyleFlag
            {
                BevelJoin_FlatCap = 0,
                RoundJoin_RoundCap = 1,
                BevelJoin_PointedCap = 2,
                RoundJoin_PointedCap = 3,
                MiterJoin_FlatCap = 4,
                MiterJoin_PointedCap = 6
            };

            enum DoubleLineFlags
            {
                DoubleFlagFillColorOn = 1,
                DoubleFlagBackgroundColorOn = 2,
            };

            enum DoubleLineMode
            {
                DoubleLineOff = 0,
                DoubleLineContinuous = 1,
                DoubleLineLeftBorderDashed = 2,
                DoubleLineBordersDashed = 3,
                DoubleLineAllDashed = 4,
            };
        };

        struct LineSymbol
        {
            using Element = PointSymbolElement;

            BaseSymbol base;
            LineSymbolGeneric generic;

            Element* element;
        };

        struct LineTextSymbol
        {
            BaseSymbol base;

            Ocad::Generic::PascalString<31> text;
            BasicTextAttributes basic;
            FramingAttributes framing;
        };

        struct AreaSymbolGeneric
        {
            u16 fill_color;
            u16 hatch_mode;
            u16 hatch_color;
            u16 hatch_line_width;
            u16 hatch_dist;
            i16 hatch_angle_1;
            i16 hatch_angle_2;
            u8 fill_on_V9; /// \since V9
            u8 border_on_V9; /// \since V9
            u8 structure_mode;
            u8 structure_draw_V12; /// \since V12
            u16 structure_width;
            u16 structure_height;
            i16 structure_angle;
        };

        struct AreaSymbol
        {
            ;
            using Element = PointSymbolElement;

            BaseSymbol base;

            u16 RESERVED_MEMBER1; // formerly known as area_flags
            u16 fill_on;
            AreaSymbolGeneric common;
            u16 RESERVED_MEMBER2;
            u16 data_size;

            Element* begin_of_elements[1];
        };

        struct TextSymbol
        {
            BaseSymbol base;

            Ocad::Generic::PascalString<31> font_name;
            BasicTextAttributes basic;
            SpecialTextAttributes special;
            u16 RESERVED_MEMBER;
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
            u16 RESERVED_MEMBER1[2];;
            u16 unnumbered_cells;
            Ocad::Generic::PascalString<3> unnumbered_text;
            u16 RESERVED_MEMBER2;
            Ocad::Generic::PascalString<31> RESERVED_MEMBER3;
            u16 RESERVED_MEMBER4;
            u16 font_size_V10; /// Since V10
            u16 RESERVED_MEMBER5[4];
        };


        struct ObjectIndex
        {
            Ocad::Generic::OcadCoord bottom_left_bound;
            Ocad::Generic::OcadCoord top_right_bound;
            u32 pos;
            u32 size;
            i32 symbol;
            u8 type;
            u8 encryption_mode_V11; /// Since V11
            u8 status;
            u8 view_type;
            u16 color;
            u16 group_V11; /// Since V11
            u16 layer;
            u8 layout_font_V11_ONLY; /// only in V11
            u8 RESERVED_MEMBER;
        };

        struct ObjectIndexBlock
        {
            u32 nextBlock;
            ObjectIndex* table;
        };

        struct Object
        {
            using IndexEntryType = ObjectIndex;

            i32 symbol;
            u8 type;
            u8 customer_V11; /// \since V11
            i16 angle;
            u32 num_items;
            u16 num_text;
            u8 mark_V11; /// \since V11
            u8 snapping_mark_V11; /// \since V11
            u32 color;
            u16 line_width;
            u16 diam_flags;
            // The usage of the following 16 bytes has changed significantly in the
            // versions 9 to 12. This is an abstraction, capturing what seems most
            // relevant.
            u32 RESERVED_MEMBER1; /// V11: Server object ID
            u32 height_V11; /// \since V11; unit: 1/256 mm
            u32 RESERVED_MEMBER2;
            u32 height_V10_ONLY; /// V10 only; unit: mm

            Ocad::Generic::OcadCoord coords[1];
        };

        struct Format
        {
            constexpr static u16 version = 9;
            using FileHeader = FileHeader;
            using BaseSymbol = BaseSymbol;
            using PointSymbol = PointSymbol;
            using LineSymbol = LineSymbol;
            using AreaSymbol = AreaSymbol;
            using TextSymbol = TextSymbol;
            using LineTextSymbol = LineTextSymbol;
            using RectangleSymbol = RectSymbol;
            using Object = Object;
            using Encoding = Generic::Custom8BitEncoding;
        };
    };
}


#endif //OCAD_OCDTYPESV9_H
