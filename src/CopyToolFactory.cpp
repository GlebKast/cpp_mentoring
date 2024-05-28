#include "CopyToolFactory.h"

ReturnCode CopyToolFactory::copy(int argc, char *const argv[]) const
{
    spdlog::trace("CopyToolFactory::copy()");
    
    ICopyTool *product = this->createCopyTool();
    ReturnCode result = product->copy(argc, argv);
    delete product;
    return result;
}