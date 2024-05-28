/*
Your task is to implement simple 2 threaded "copy" tool.

Tool should be implemented as a console application.
Tool should accept 2 parameters source filename and target filename.
Copying logic should be organized with help of 2 threads.
First thread should read the data from source file.
Second thread should write the data to the target file.
*/

#pragma once
#include "ICopyTool.h"
#include "MultithreadingChunkController.h"
#include "StreamFileReader.h"
#include "StreamFileWriter.h"
#include "MultithreadingCopyToolValidator.h"

namespace multithreading
{

class MultithreadingCopyTool : public ICopyTool
{
public:
    MultithreadingCopyTool();

    MultithreadingCopyTool(
      std::shared_ptr<IChunkController> chunkController,
      std::shared_ptr<fileManager::IFileReader> fileReader,
      std::shared_ptr<fileManager::IFileWriter> fileWriter,
      std::shared_ptr<ICopyToolValidator> copyToolValidator);

    virtual ~MultithreadingCopyTool() override = default;

    ReturnCode copy(int argc, char *const argv[]) override;

    ReturnCode read() override;

    ReturnCode write() override;

private:
    std::vector<char> m_chunk;
    std::shared_ptr<IChunkController> m_chunkController;
    std::shared_ptr<fileManager::IFileReader> m_fileReader;
    std::shared_ptr<fileManager::IFileWriter> m_fileWriter;
    std::shared_ptr<ICopyToolValidator> m_copyToolValidator;
};

}    // namespace multithreading