//
// Created by ace on 2026-04-25.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OMAP_H
#define PROCEDURALFOLIAGEGENERATOR_OMAP_H

namespace Omap
{
    enum SymbolType
    {
        SymbolTypePoint = 1,
        SymbolTypeLine = 2,
        SymbolTypeArea = 4,
    };

    enum ObjectStatus
    {
        ObjectDeleted = 0,
        ObjectNormal = 1,
        ObjectHidden = 2,
        ObjectDeletedForUndo = 3
    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_OMAP_H