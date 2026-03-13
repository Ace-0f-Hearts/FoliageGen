//
// Created by ace on 2026-01-28.
//

#ifndef GENERATOR_MAP_H
#define GENERATOR_MAP_H
#include <array>
#include <memory>
#include <string>
#include <vector>

#include <orienteering/symbol.h>
#include <orienteering/object.h>


namespace Orienteering
{
    using string = std::string;

    class Symbol;
    struct Object;
    struct AreaSymbol;
    struct LineSymbol;
    struct PointSymbol;
    struct PathObject;


    class Map
    {
    public:

        Map() = default;
        ~Map() = default;

        Map(const Map&) = default;

        void AppendSymbol(std::unique_ptr<Symbol> symbol);
        void AppendObject(std::unique_ptr<Object> obj);

        [[nodiscard]] size_t GetSymbolOfTypeAmount(SymbolType type) const;
        [[nodiscard]] size_t GetObjectOfTypeAmount(ObjectType type) const;
        [[nodiscard]] size_t GetSymbolAmount() const;
        [[nodiscard]] size_t GetObjectAmount() const;
        [[nodiscard]] size_t GetObjectOfSymbolAmount(Symbol* symbol) const;

        Symbol* GetSymbolById(size_t id);

        void ClearSymbols();
        void ClearObjects();
        void Clear();

        void RemoveObjectsOfSymbol(Symbol& symbol);
        void RemoveSymbol(Symbol& symbol);
    private:
        std::vector<std::unique_ptr<Symbol>> symbols_;
        std::vector<std::unique_ptr<Object>> objects_;

    };
}



#endif //GENERATOR_MAP_H