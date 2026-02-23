//
// Created by ace on 2026-01-26.
//

#ifndef GENERATOR_SYMBOLS_H
#define GENERATOR_SYMBOLS_H
#include <string>
#include <array>
#include <cstdint>

namespace Orienteering
{
    using string = std::string;


    constexpr static auto NUMBERCOMPONENTS = 3u;


    enum SymbolType{
        Obstructing, /// prevents the growth of plants around it and inside of it
        Cultivated, /// reduces the amount of randomization in the seed positions
        Directional, /// introduces some bias to the direction of randomization
      };


    class Symbol{
    public:
        Symbol() = default;
        virtual ~Symbol() = default;

        void name(string& name);
        void number(uint32_t value);

        string& name();
        [[nodiscard]] string name() const;

        uint32_t& number();
        [[nodiscard]] uint32_t number() const;
    private:
        string name_;
        /**
         * Unique ID to symbol
         */
        uint32_t id_;
        float radius_;
        uint8_t flags_;



    };
}


#endif //GENERATOR_SYMBOLS_H