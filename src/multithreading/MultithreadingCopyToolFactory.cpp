#include "MultithreadingCopyToolFactory.h"

ICopyTool *MultithreadingCopyToolFactory::FactoryMethod() const
{
    return new multithreading::MultithreadingCopyTool();
}