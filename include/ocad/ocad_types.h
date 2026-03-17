//
// Created by ace on 2026-01-26.
//

#ifndef OCAD_OCDTYPES_H
#define OCAD_OCDTYPES_H
#include <cstdint>

namespace Ocad
{
    class Generic
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
            u16 vendor_mark = 0x0cad;
            u8 file_type{};
            u8 file_status{}; /// Since V9
            u16 version{};
            u8 subversion{};
            u8 subsubversion{}; /// Since V10
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


        struct OcadCoord
        {
            i32 x;
            i32 y;

            [[nodiscard]] bool IsFirstCurvePoint() const;
            [[nodiscard]] bool IsSecondCurvePoint() const;
            [[nodiscard]] bool NoLeftLine() const;
            [[nodiscard]] bool IsCornerPoint() const;
            [[nodiscard]] bool IsFirstHolePoint() const;
            [[nodiscard]] bool NoRightLine() const;
            [[nodiscard]] bool IsDashPoint() const;


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
        struct Utf8Encoding{};
    };


}

inline std::ostream& operator<<(std::ostream& os, Ocad::Generic::OcadCoord coord)
{
    os << "(" << coord.x << "," << coord.y << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Ocad::Generic::FileHeaderGeneric header)
{
    os << "( " << header.vendor_mark << ";" << header.file_type << ";" << header.file_status << ";" << header.version << ";" << header.subversion << ";" << header.subsubversion << ")";
    return os;
}

#endif //OCAD_OCDTYPES_H
