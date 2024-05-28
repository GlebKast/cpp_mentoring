#include "MultiprocessingCopyTool.h"

namespace multiprocessing
{
using namespace fileManager;

MultiprocessingCopyTool::MultiprocessingCopyTool() :
    m_chunkController(nullptr),
    m_fileReader(nullptr),
    m_fileWriter(nullptr),
    m_copyToolValidator(nullptr)
{
    spdlog::trace("MultiprocessingCopyTool::MultiprocessingCopyTool()");
}

MultiprocessingCopyTool::MultiprocessingCopyTool(
  std::shared_ptr<IChunkController> chunkController,
  std::shared_ptr<IFileReader> fileReader,
  std::shared_ptr<IFileWriter> fileWriter,
  std::shared_ptr<ICopyToolValidator> copyToolValidator) :
    m_chunkController(chunkController),
    m_fileReader(fileReader),
    m_fileWriter(fileWriter),
    m_copyToolValidator(copyToolValidator)
{
    spdlog::trace(
      "MultiprocessingCopyTool::MultiprocessingCopyTool(chunkController, fileReader, fileWriter)");
}

ReturnCode MultiprocessingCopyTool::copy(int argc, char *const argv[])
{
    spdlog::trace("MultiprocessingCopyTool::copy()");

    if(m_copyToolValidator == nullptr)
    {
        m_copyToolValidator = std::make_shared<MultiprocessingCopyToolValidator>();
    }

    if(m_copyToolValidator->validateUserInput(argc, argv) == false)
    {
        return ReturnCode::InvalidArguments;
    }

    std::string command(argv[1]);
    std::string sourceFileName(argv[2]);
    std::string targetFileName(argv[3]);
    std::string sharedMemoryName = argv[4];

    m_chunk.resize(CHUNK_SIZE);

    if(m_chunkController == nullptr)
    {
        m_chunkController = std::make_shared<MultiprocessingChunkController>(sharedMemoryName);
    }

    if(command == "read")
    {
        if(m_fileReader == nullptr)
        {
            m_fileReader = std::make_shared<StreamFileReader>(sourceFileName.c_str());
        }

        return read();
    }
    else if(command == "write")
    {
        if(m_fileWriter == nullptr)
        {
            m_fileWriter = std::make_shared<StreamFileWriter>(targetFileName.c_str());
        }

        return write();
    }

    return ReturnCode::CopyError;
}

ReturnCode MultiprocessingCopyTool::read()
{
    spdlog::trace("MultiprocessingCopyTool::read()");

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

ReturnCode MultiprocessingCopyTool::write()
{
    spdlog::trace("MultiprocessingCopyTool::write()");

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

}    // namespace multiprocessing