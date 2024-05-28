#pragma once

#include "Common.h"

class IChunkController
{
public:
    virtual ~IChunkController() = default;

    virtual bool get(char *chunk, std::size_t &actualSize) = 0;
    virtual bool put(const char *chunk, std::size_t actualSize) = 0;

    virtual bool notifyCompleted() = 0;
    virtual bool isCompleted() const = 0;
};