#include "MultiprocessingCopyToolFactory.h"
#include "MultithreadingCopyToolFactory.h"

int main(int argc, char *const argv[])
{
    std::unique_ptr<CopyToolFactory> cp = std::make_unique<MultiprocessingCopyToolFactory>();

    ReturnCode ret = cp->copy(argc, argv);

    return ret == ReturnCode::Success ? 0 : 1;
}