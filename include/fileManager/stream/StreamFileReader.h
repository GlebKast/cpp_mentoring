#pragma once

#include "IFileReader.h"

namespace fileManager
{

class StreamFileReader : public IFileReader
{
public:
    StreamFileReader(const char *fileName);
    virtual ~StreamFileReader() override = default;

    bool read(char *s, std::size_t bufferSize, std::size_t &actualSize) override;
    bool isEndOfFile() const override;

private:
    std::ifstream m_file;
};

}    // namespace fileManager