//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#include "object.h"

namespace Orienteering
{

    class PathObject : public Object
    {
    public:
        PathObject() = delete;
        PathObject(Symbol* symbol);
        ~PathObject() override = default;

        [[nodiscard]] bool IsClosed() const;
        [[nodiscard]] bool Contains(Spatial2D) const;

        void SetPoints(std::vector<Spatial2D>);
        void AddPoint(Spatial2D);
        void RemovePoint(Spatial2D);


    protected:

    private:


    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H