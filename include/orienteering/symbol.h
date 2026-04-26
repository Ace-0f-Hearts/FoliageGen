//
// Created by ace on 2026-01-26.
//

#ifndef GENERATOR_SYMBOLS_H
#define GENERATOR_SYMBOLS_H
#include <string>
#include <array>
#include <cstdint>
#include <memory>
#include "map_color.h"
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
        Cultivated  = 0x2, /// reduces the amount of randomization in the seed positions //TODO
        Directional = 0x4, /// introduces some bias to the direction of randomization //TODO
        Irrelevant = 0x8, /// symbol defining features on the map itself, used for isolating empty areas which would be designated as forests otherwise
    };


    class Symbol{
    public:
        Symbol();
        ~Symbol();

        void SetName(const string& name);
        void SetNumber(uint32_t value);

        string& GetName();
        [[nodiscard]] string GetName() const;

        uint32_t& GetNumber();
        [[nodiscard]] uint32_t GetNumber() const;
        bool operator==(const Symbol& other) const;

        [[nodiscard]] uint32_t GetId() const;
        void SetId(uint32_t value);

        void SetRadiusOfInfluence(float radius);
        [[nodiscard]] float GetRadiusOfInfluence() const;

        [[nodiscard]] const MapColor* GetColor() const;


        [[nodiscard]] uint8_t flags() const;

        [[nodiscard]] bool HasColor() const;
        [[nodiscard]] bool IsArea() const;
        [[nodiscard]] bool IsPath() const;
        [[nodiscard]] bool IsPoint() const;
        [[nodiscard]] bool IsIrrelevant() const;
        [[nodiscard]] bool IsCultivated() const;
        [[nodiscard]] bool IsObstructing() const;
        [[nodiscard]] bool IsDirectional() const;

        void type(SymbolType type);
        [[nodiscard]] SymbolType type() const;

        void SetCultivated(bool cultivated);
        void SetObstructing(bool obstructing);
        void SetDirectional(bool directional);
        void SetMapSymbol(bool mapsymbol);

        void SetColor(const MapColor* color);
        void SetColor(MapColor* color);
        void SetColor(std::shared_ptr<MapColor>& color);



    private:
        std::shared_ptr<MapColor> color_;
        string name_;
        /**
         * Unique ID to symbol
         */
        uint32_t id_;
        uint8_t flags_;
        float radius_of_influence_;

        SymbolType type_;
    };
}


#endif //GENERATOR_SYMBOLS_H