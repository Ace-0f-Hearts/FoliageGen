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




    enum SymbolType{
        PointS = 0x1,
        PathS = 0x2,
        AreaS = 0x4,
      };

    enum SymbolFlag
    {
        Obstructing = 0x1, /// prevents the growth of plants around it and inside of it
        Cultivated  = 0x2, /// reduces the amount of randomization in the seed positions
        Directional = 0x4, /// introduces some bias to the direction of randomization
    };


    class Symbol{
    public:
        Symbol();
        ~Symbol() = default;

        void name(const string& name);
        void number(uint32_t value);

        string& name();
        [[nodiscard]] string name() const;

        uint32_t& number();
        [[nodiscard]] uint32_t number() const;
        bool operator==(const Symbol& other) const;

        [[nodiscard]] uint32_t id() const;
        void id(uint32_t value);

        [[nodiscard]] uint8_t flags() const;

        [[nodiscard]] bool IsArea() const;
        [[nodiscard]] bool IsPath() const;
        [[nodiscard]] bool IsPoint() const;
        [[nodiscard]] bool IsCultivated() const;
        [[nodiscard]] bool IsObstructing() const;
        [[nodiscard]] bool IsDirectional() const;

        void type(SymbolType type);
        SymbolType type();

        void SetCultivated(bool cultivated);
        void SetObstructing(bool obstructing);
        void SetDirectional(bool directional);


    private:
        string name_;
        /**
         * Unique ID to symbol
         */
        uint32_t id_;
        uint8_t flags_;

        SymbolType type_;
    };
}


#endif //GENERATOR_SYMBOLS_H