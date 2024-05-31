#include "MultiprocessingCopyToolFactory.h"
#include "MultithreadingCopyToolFactory.h"
#include "Utils.h"

int main(int argc, char *const argv[])
{
    spdlog::set_level(spdlog::level::info);

    std::shared_ptr<CopyToolFactory> cp = std::make_shared<MultithreadingCopyToolFactory>();

    ReturnCode ret = cp->copy(argc, argv);

    spdlog::info("Return code: {}", returnCodeToString(ret));

    return ret == ReturnCode::Success ? 0 : 1;
}