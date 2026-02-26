//
// Created by ace on 2026-01-28.
//

#ifndef GENERATOR_MAP_H
#define GENERATOR_MAP_H
#include <array>
#include <string>
#include <vector>



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

        void AddSymbol(Symbol* symbol);

        void AddObject(Object* obj);

    private:
        std::vector<Symbol*> symbols_;
        std::vector<Object*> objects_;

    };
}



#endif //GENERATOR_MAP_H