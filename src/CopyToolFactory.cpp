#include "CopyToolFactory.h"

ReturnCode CopyToolFactory::copy(int argc, char *const argv[]) const
{
    ICopyTool *product = this->FactoryMethod();
    ReturnCode result = product->copy(argc, argv);
    delete product;
    return result;
}