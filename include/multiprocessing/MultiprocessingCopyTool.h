/*
Your task is to rework the copy tool in the next manner:
1. Your tool should now accept 3 parameters <source> <destination> <name of shared memory object>
2. When run your tool by script twice.
2.1. First exemplar of executed tools should act as a reader of source file
2.2. Second exemplar of executed tools should act as a writer of destination file
3. Use 'placement' new for organize reading to shared memory. As well as writing.
4. Try to use constructor/destructor of a class - to notify about capture/release memory after read/write operation completion
Discover how to organize interprocess synchronization (windows, Linux, MacOS)
*/

#pragma once

#include "ICopyTool.h"
#include "MultiprocessingChunkController.h"
#include "StreamFileReader.h"
#include "StreamFileWriter.h"
#include "MultiprocessingCopyToolValidator.h"

namespace multiprocessing
{

class MultiprocessingCopyTool : public ICopyTool
{
public:
    MultiprocessingCopyTool();

    MultiprocessingCopyTool(
      std::shared_ptr<IChunkController> chunkController,
      std::shared_ptr<fileManager::IFileReader> fileReader,
      std::shared_ptr<fileManager::IFileWriter> fileWriter,
      std::shared_ptr<ICopyToolValidator> copyToolValidator);

    virtual ~MultiprocessingCopyTool() override = default;

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

}    // namespace multiprocessing

// add class for controling
// add mutex on shared memory
// boost options reading
