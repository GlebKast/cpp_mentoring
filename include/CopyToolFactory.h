#pragma once

#include "ICopyTool.h"

class CopyToolFactory
{
public:
    virtual ~CopyToolFactory() = default;
    virtual ICopyTool *createCopyTool() const = 0;

    ReturnCode copy(int argc, char *const argv[]) const;
};