//
// Created by ace on 2026-01-26.
//

#ifndef GENERATOR_OCDTYPES_H
#define GENERATOR_OCDTYPES_H
#include <cstdint>

namespace Ocd
{
    class OcdTypesGeneric
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

        struct FileHeaderGeneric
        {
            u16 ocadMark = 0x0cad;
            u8 fileType;
            u8 fileStatus; /// Since V9
            uint16_t version;
            u8 subversion;
            u8 subsubversion; /// Since V10
        };

        template <unsigned int N>
        struct PascalString
        {
            unsigned char length;
            char data[N];
        };

        /// N must be greater or equal to 3
        template <unsigned int N>
        struct Utf8PascalString
        {
            unsigned char length;
            char data[N];
        };

        /// N must be greater or equal to 2
        template <std::size_t N>
        struct Utf16String
        {
            char data[N];
        };


        struct OcdCoord
        {
            u32 x;
            u32 y;

            enum XFlags
            {
                FlagFirstCurvePoint = 0x01,
                FlagSecondCurvePoint = 0x02,
                FlagNoLeftLine = 0x04,
            };

            enum YFlags
            {
                FlagCornerPoint = 0x01,
                FlagFirstHolePoint = 0x02,
                FlagNoRightLine = 0x04,
                FlagDashPoint = 0x08, /// V7 only
            };
        };

        struct Custom8BitEncoding
        {
        };
    };
}


#endif //GENERATOR_OCDTYPES_H
