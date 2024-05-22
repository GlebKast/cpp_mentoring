#include "MultithreadingCopyTool.h"

using namespace multithreading;

MultithreadingCopyTool::MultithreadingCopyTool() :
    m_chunk(CHUNK_SIZE), m_completed(false), m_chunkReady(false) {};

ReturnCode MultithreadingCopyTool::copy(int argc, char *const argv[])
{
    if(argc != 3)
    {
        std::cout << "Error. Please, provide 2 arguments: source and target file names.";
        return ReturnCode::InvalidArguments;
    }
    if(! std::filesystem::exists(argv[1]))
    {
        std::cout << "Error. File to copy doesn't exist.";
        return ReturnCode::ReadError;
    }

    const std::string sourceName {argv[1]};
    const std::string targetName {argv[2]};

    std::filesystem::path filePath(targetName);
    if(filePath.has_parent_path() && ! std::filesystem::exists(filePath.parent_path()))
    {
        return ReturnCode::WriteError;
    }

    auto readF = std::async(std::launch::async, &ICopyTool::read, this, sourceName);
    auto writeF = std::async(std::launch::async, &ICopyTool::write, this, targetName);

    if(readF.get() != ReturnCode::Success)
    {
        return ReturnCode::ReadError;
    }
    else if(writeF.get() != ReturnCode::Success)
    {
        return ReturnCode::WriteError;
    }
    else
    {
        std::filesystem::path p {sourceName};
        std::filesystem::path pC {targetName};
        if(
          ! std::filesystem::exists(p) || ! std::filesystem::exists(pC) ||
          std::filesystem::file_size(p) != std::filesystem::file_size(pC))
        {
            return ReturnCode::CopyError;
        }
    }

    return ReturnCode::Success;
}

ReturnCode MultithreadingCopyTool::read(const std::string &sourceName)
{
    std::ifstream file(sourceName, std::ifstream::binary);

    if(! file)
    {
        m_completed = true;
        m_chunkCV.notify_one();
        return ReturnCode::ReadError;
    }

    while(! file.eof())
    {
        std::unique_lock lk(m_chunkMutex);

        file.read(m_chunk.data(), m_chunk.size());

        if(file.gcount() < CHUNK_SIZE)
        {
            m_chunk.resize(file.gcount());
        }

        m_chunkReady = true;

        lk.unlock();

        m_chunkCV.notify_one();

        lk.lock();

        m_chunkCV.wait(lk, [this] { return m_chunkReady == false; });
    }

    m_completed = true;
    m_chunkCV.notify_one();

    return ReturnCode::Success;
}

ReturnCode MultithreadingCopyTool::write(const std::string &targetName)
{
    std::ofstream file(targetName, std::ofstream::binary);

    if(! file)
    {
        return ReturnCode::WriteError;
    }

    while(! m_completed)
    {
        std::unique_lock lk(m_chunkMutex);

        m_chunkCV.wait(lk, [this] { return m_chunkReady == true || m_completed == true; });

        if(m_completed)
        {
            return ReturnCode::Success;
        }

        file.write(m_chunk.data(), m_chunk.size());

        m_chunkReady = false;

        lk.unlock();

        m_chunkCV.notify_one();
    }

    return ReturnCode::Success;
}