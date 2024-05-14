/*
Your task is to implement simple 2 threaded "copy" tool.

Tool should be implemented as a console application.
Tool should accept 2 parameters source filename and target filename.
Copying logic should be organized with help of 2 threads.
First thread should read the data from source file.
Second thread should write the data to the target file.
*/

#pragma once
#include "common.h"

class CopyTool
{
public:
    CopyTool() : m_completed(false), m_chunkReady(false)
    {
        m_chunk.resize(CHUNK_SIZE);
    }

    ReturnCode copy(const std::string &sourceName, const std::string &targetName)
    {
        std::filesystem::path filePath(targetName);
        if (filePath.has_parent_path() && !std::filesystem::exists(filePath.parent_path())) 
        {
            return ReturnCode::WriteError;
        }

        auto readF = std::async(std::launch::async, &CopyTool::read, this, sourceName);
        auto writeF = std::async(std::launch::async, &CopyTool::write, this, targetName);

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
            std::filesystem::path p{sourceName};
            std::filesystem::path pC{targetName};
            if(!std::filesystem::exists(p) ||  !std::filesystem::exists(pC) || std::filesystem::file_size(p) != std::filesystem::file_size(pC))
            {
                return ReturnCode::CopyError;
            }
        }

        return ReturnCode::Success;
    }
private:
    ReturnCode read(const std::string &sourceName)
    {
        std::ifstream file(sourceName, std::ifstream::binary);

        if(!file)
        {
            m_completed = true;
            m_chunkCV.notify_one();
            return ReturnCode::ReadError;
        }

        while(!file.eof())
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

            m_chunkCV.wait(lk, [this]{return m_chunkReady == false;});
        }

        m_completed = true;
        m_chunkCV.notify_one();

        return ReturnCode::Success;
    }

    ReturnCode write(const std::string &targetName)
    {
        std::ofstream file(targetName, std::ofstream::binary);

        if(!file)
        {
            return ReturnCode::WriteError;
        }

        while(true)
        {
            std::unique_lock lk(m_chunkMutex);

            m_chunkCV.wait(lk, [this]{return m_chunkReady == true || m_completed == true;});

            if(m_completed == true)
            {
                break;
            }

            file.write(m_chunk.data(), m_chunk.size());

            m_chunkReady = false;

            lk.unlock();

            m_chunkCV.notify_one();
        }

        return ReturnCode::Success;
    }

private:
    std::mutex m_chunkMutex;
    std::condition_variable m_chunkCV;
    std::vector<char> m_chunk;
    std::atomic<bool> m_completed;
    std::atomic<bool> m_chunkReady;
};

void runMutithreadingCopyTool(int argc, char *const argv[])
{
    if(argc != 3)
    {
        std::cout << "Error. Please, provide 2 arguments: source and target file names.";
        return;
    }
    if(!std::filesystem::exists(argv[1]))
    {
        std::cout << "Error. File to copy doesn't exist.";
        return;
    }

    CopyTool cp;

    ReturnCode ret = cp.copy(argv[1], argv[2]);

    if(ret == ReturnCode::Success)
    {
        std::cout << "Copy completed successfully." << std::endl;
    }
    else
    {
        std::cout << "Error. Copy failed with code '" << static_cast<int>(ret) << "'" << std::endl;
    }
}