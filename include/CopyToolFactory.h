#pragma once

#include "ICopyTool.h"
#include <optional>

class CopyToolFactory
{
public:
    virtual ~CopyToolFactory() {};
    virtual ICopyTool *FactoryMethod() const = 0;

    ReturnCode copy(int argc, char *const argv[]) const;
};