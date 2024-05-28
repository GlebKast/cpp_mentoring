#pragma once

#include "IFileWriter.h"

namespace fileManager
{

class StreamFileWriter : public IFileWriter
{
public:
    StreamFileWriter(const char *fileName);
    virtual ~StreamFileWriter() override = default;

    bool write(const char *s, std::size_t actualSize) override;

private:
    std::ofstream m_file;
};

}    // namespace fileManager