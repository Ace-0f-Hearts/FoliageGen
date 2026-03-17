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
        [[nodiscard]] char Key() const {return current_key;}
        [[nodiscard]] std::string Value() const;
        [[nodiscard]] bool AtEnd() const {return next == -1;}

        static constexpr char KNo_key() {return 0;}
    private:
        std::string param_;
        int pos;
        int next;
        char current_key;
    };

}

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_PARAMATER_STREAM_READER_H