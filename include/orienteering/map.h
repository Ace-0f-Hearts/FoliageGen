//
// Created by ace on 2026-01-28.
//

#ifndef GENERATOR_MAP_H
#define GENERATOR_MAP_H
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <orienteering/symbol.h>
#include <orienteering/object.h>

#include "map_color.h"


namespace Orienteering
{
    using string = std::string;

    class Symbol;
    class Object;
    struct AreaSymbol;
    struct LineSymbol;
    struct PointSymbol;
    struct PathObject;

    template <class O>
        class FilterIterator
    {
    public:
        FilterIterator(const std::vector<std::unique_ptr<O>>* objects, std::function<bool(O*)> predicate)
        {
            if (objects == nullptr)
                throw std::logic_error("FilterIterator: objects is nullptr");

            predicate_ = predicate;
            objects_ = objects;
        }

        O* next()
        {
            auto it = objects_->cbegin();
            for (; it != objects_->cend() && !predicate_(*it); ++it)
            {
                // Find next item that matches our predicate
            }

            if (it == objects_->cend())
                return nullptr;

            return objects_->at(idx_++).get();
        }
    private:
        std::function<bool(O*)> predicate_;
        size_t idx_ = 0;
        const std::vector<std::unique_ptr<O>>* objects_;
    };

    class OrienteeringMap
    {
    public:

        OrienteeringMap() = default;
        ~OrienteeringMap();

        OrienteeringMap(const OrienteeringMap&) = default;

        void AppendColor(std::shared_ptr<MapColor> color);
        void AppendSymbol(std::shared_ptr<Symbol> symbol);
        void AppendObject(std::unique_ptr<Object> obj);

        [[nodiscard]] bool ObjectsCanBeOrdered();

        [[nodiscard]] size_t GetColorsAmount() const;
        [[nodiscard]] size_t GetSymbolOfTypeAmount(SymbolType type) const;
        [[nodiscard]] size_t GetObjectOfTypeAmount(ObjectType type) const;
        [[nodiscard]] size_t GetSymbolAmount() const;
        [[nodiscard]] size_t GetObjectAmount() const;
        [[nodiscard]] size_t GetObjectOfSymbolAmount(Symbol* symbol) const;

        [[nodiscard]] std::vector<Object*> GetObjects() const;
        [[nodiscard]] std::vector<Object*> GetObjectsOfType(ObjectType type) const;
        [[nodiscard]] std::vector<Object*> GetObjectsOfType(uint8_t type) const;
        [[nodiscard]] FilterIterator<Object> GetObjectsOf(ObjectType oType, SymbolFlag sFlag) const;

        //TODO: Create an iterator with filtering




        [[nodiscard]] std::vector<Object*> GetObstructingObjects() const;
        [[nodiscard]] std::vector<Object*> GetFreeAreas() const;
        [[nodiscard]] std::vector<Object*> GetIrrelevantFeatures() const;

        Symbol* GetSymbolById(size_t id);
        [[nodiscard]] BoundingBox2D GetBoundingBox() const;

        [[nodiscard]] std::vector<std::shared_ptr<MapColor>>& colors();
        [[nodiscard]] std::vector<std::shared_ptr<Symbol>>& symbols();
        [[nodiscard]] std::vector<std::unique_ptr<Object>>& objects();
        void UpdateBoundingBox();

        void ClearSymbols();
        void ClearObjects();
        void Clear();
        void ClearObjectsOfType(ObjectType type);
        void ClearObjectsOfType(uint8_t type);
        void ClearObjectsOfFlag(SymbolFlag flag);
        void ClearObjectsOfFlag(uint8_t type);

        void RemoveObjectsOfSymbol(Symbol& symbol);
        void RemoveSymbol(Symbol& symbol);
    private:
        std::vector<std::shared_ptr<MapColor>> colors_;
        std::vector<std::shared_ptr<Symbol>> symbols_;
        std::vector<std::unique_ptr<Object>> objects_;
        BoundingBox2D bounding_box_;
    };
}



#endif //GENERATOR_MAP_H