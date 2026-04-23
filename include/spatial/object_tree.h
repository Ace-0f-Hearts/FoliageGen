//
// Created by ace on 2026-03-13.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OBJECT_TREE_H
#define PROCEDURALFOLIAGEGENERATOR_OBJECT_TREE_H
#include <memory>

#include "orienteering/object.h"

class ObjectTree
{
public:
    ObjectTree();
    ObjectTree(std::vector<std::shared_ptr<Object>> objects);

    void BulkLoad(std::vector<std::shared_ptr<Object>> objects);
    void Clear();

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<size_t> index_tree_;
}
#endif //PROCEDURALFOLIAGEGENERATOR_OBJECT_TREE_H