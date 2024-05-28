#include "MultithreadingCopyTool.h"

namespace multithreading
{
using namespace fileManager;

MultithreadingCopyTool::MultithreadingCopyTool() :
    m_chunkController(nullptr), m_fileReader(nullptr), m_fileWriter(nullptr), m_copyToolValidator(nullptr)
{
    spdlog::trace("MultithreadingCopyTool::MultithreadingCopyTool()");
}

MultithreadingCopyTool::MultithreadingCopyTool(
  std::shared_ptr<IChunkController> chunkController,
  std::shared_ptr<IFileReader> fileReader,
  std::shared_ptr<IFileWriter> fileWriter,
  std::shared_ptr<ICopyToolValidator> copyToolValidator) :
    m_chunkController(chunkController), m_fileReader(fileReader), m_fileWriter(fileWriter), m_copyToolValidator(copyToolValidator)
{
    spdlog::trace(
      "MultithreadingCopyTool::MultithreadingCopyTool(chunkController, fileReader, fileWriter)");
}

ReturnCode MultithreadingCopyTool::copy(int argc, char *const argv[])
{
    spdlog::trace("MultithreadingCopyTool::copy()");

    if(m_copyToolValidator == nullptr)
    {
        m_copyToolValidator = std::make_shared<MultithreadingCopyToolValidator>();
    }

    if(m_copyToolValidator->validateUserInput(argc, argv) == false)
    {
        return ReturnCode::InvalidArguments;
    }

    const std::string sourceFileName {argv[1]};
    const std::string targetFileName {argv[2]};

    m_chunk.resize(CHUNK_SIZE);

    if(m_chunkController == nullptr)
    {
        m_chunkController = std::make_shared<MultithreadingChunkController>();
    }
    if(m_fileReader == nullptr)
    {
        m_fileReader = std::make_shared<StreamFileReader>(sourceFileName.c_str());
    }
    if(m_fileWriter == nullptr)
    {
        m_fileWriter = std::make_shared<StreamFileWriter>(targetFileName.c_str());
    }

    auto readF = std::async(std::launch::async, &ICopyTool::read, this);
    auto writeF = std::async(std::launch::async, &ICopyTool::write, this);

    if(readF.get() != ReturnCode::Success)
    {
        return ReturnCode::ReadError;
    }
    else if(writeF.get() != ReturnCode::Success)
    {
        return ReturnCode::WriteError;
    }
   
    return ReturnCode::Success;
}

ReturnCode MultithreadingCopyTool::read()
{
    spdlog::trace("MultithreadingCopyTool::read()");

    std::size_t actualSize = 0;

    while(m_fileReader->isEndOfFile() == false)
    {
        if(m_fileReader->read(m_chunk.data(), CHUNK_SIZE, actualSize) == false)
        {
            return ReturnCode::ReadError;
        }

        if(actualSize < CHUNK_SIZE)
        {
            m_chunk.resize(actualSize);
        }

        if(m_chunkController->put(m_chunk.data(), m_chunk.size()) == false)
        {
            return ReturnCode::ReadError;
        }
    }

    m_chunkController->notifyCompleted();

    return ReturnCode::Success;
}

ReturnCode MultithreadingCopyTool::write()
{
    spdlog::trace("MultithreadingCopyTool::write()");

    std::size_t actualSize = 0;

    while(true)
    {
        if(m_chunkController->get(m_chunk.data(), actualSize) == false)
        {
            return ReturnCode::WriteError;
        }

        if(m_chunkController->isCompleted())
        {
            break;
        }

        if(m_fileWriter->write(m_chunk.data(), actualSize) == false)
        {
            return ReturnCode::WriteError;
        }
    }

    return ReturnCode::Success;
}

}    // namespace multithreading