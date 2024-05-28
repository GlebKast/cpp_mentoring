#include "MultiprocessingCopyToolFactory.h"

ICopyTool *MultiprocessingCopyToolFactory::createCopyTool() const
{
    spdlog::trace("MultiprocessingCopyToolFactory::createCopyTool()");
    
    return new multiprocessing::MultiprocessingCopyTool();
}