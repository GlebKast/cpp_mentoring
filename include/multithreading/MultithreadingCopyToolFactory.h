#pragma once

#include "../CopyToolFactory.h"
#include "MultithreadingCopyTool.h"

class MultithreadingCopyToolFactory : public CopyToolFactory
{
public:
    ICopyTool *FactoryMethod() const override;
};