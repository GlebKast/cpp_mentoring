#pragma once

#include "../CopyToolFactory.h"
#include "MultiprocessingCopyTool.h"

class MultiprocessingCopyToolFactory : public CopyToolFactory
{
public:
    ICopyTool *FactoryMethod() const override;
};
