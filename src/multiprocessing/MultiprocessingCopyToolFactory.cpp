#include "MultiprocessingCopyToolFactory.h"

ICopyTool *MultiprocessingCopyToolFactory::FactoryMethod() const
{
    return new multiprocessing::MultiprocessingCopyTool();
}