#include "StreamFileReader.h"

namespace fileManager
{
StreamFileReader::StreamFileReader(const char *fileName)
{
    spdlog::trace("StreamFileReader::StreamFileReader()");

    m_file.open(fileName, std::ifstream::binary);
}

bool StreamFileReader::read(char *s, std::size_t bufferSize, std::size_t &actualSize)
{
    spdlog::trace("StreamFileReader::read()");

    if(! m_file)
    {
        return false;
    }

    if(! m_file.eof())
    {
        m_file.read(s, bufferSize);

        actualSize = m_file.gcount();
        
        spdlog::info("Read {} bytes. Contents: {}", actualSize, s);
        return true;
    }

    return false;
}

bool StreamFileReader::isEndOfFile() const
{   
    spdlog::trace("StreamFileReader::isEndOfFile()");

    return m_file.eof();
}

}    // namespace fileManager