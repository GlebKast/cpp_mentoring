#include "MultithreadingCopyToolFactory.h"

ICopyTool *MultithreadingCopyToolFactory::createCopyTool() const
{
    spdlog::trace("MultithreadingCopyToolFactory::createCopyTool()");
    
    return new multithreading::MultithreadingCopyTool();
}