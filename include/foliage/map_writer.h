//
// Created by ace on 2026-03-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_WRITER_H
#define PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_WRITER_H

#include "foliage_map.h"

class MapWriter
{
    public:
    static void Write(FoliageMap map);


};
static const std::string kDefault_path = "../../testing/snapshot.jpeg";

inline void MapWriter::Write(FoliageMap map)
{
    map.map().save(kDefault_path.c_str());
}


#endif //PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_WRITER_H