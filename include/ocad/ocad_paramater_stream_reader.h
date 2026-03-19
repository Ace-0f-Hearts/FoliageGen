//
// Created by ace on 2026-03-17.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_PARAMATER_STREAM_READER_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_PARAMATER_STREAM_READER_H
#include <string>

namespace Ocad
{
    class OcadParameterStreamReader
    {
    public:
        explicit OcadParameterStreamReader(const std::string& param) noexcept;

        [[nodiscard]] bool ReadNext();
        [[nodiscard]] char Key() const {return current_key_;}
        [[nodiscard]] std::string Value() const;
        [[nodiscard]] bool AtEnd() const {return next_ == std::string::npos;}

        static constexpr char KNo_key() {return 0;}
    private:
        std::string param_;
        size_t pos_;
        size_t next_;
        char current_key_;
    };

}

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_PARAMATER_STREAM_READER_H