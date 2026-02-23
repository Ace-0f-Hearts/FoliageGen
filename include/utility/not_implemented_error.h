//
// Created by ace on 2026-02-06.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_NOTIMPLEMENTED_H
#define PROCEDURALFOLIAGEGENERATOR_NOTIMPLEMENTED_H
#include <stdexcept>

class NotImplementedError : public std::logic_error
{
    public:
    NotImplementedError() : std::logic_error("Function not implemented") {}
};

#endif //PROCEDURALFOLIAGEGENERATOR_NOTIMPLEMENTED_H