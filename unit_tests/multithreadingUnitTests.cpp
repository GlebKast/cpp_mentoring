#include <gtest/gtest.h>
#include "multithreading.h"

namespace
{
class MultithreadingCopyToolTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        std::ofstream sourceFile("source.txt");
        sourceFile << "This is a test file.";
        sourceFile.close();

        std::ofstream targetFile("target.txt");
        targetFile.close();
    }

    void TearDown() override 
    {
        std::filesystem::remove("source.txt");
        std::filesystem::remove("target.txt");
    }

    bool areFilesIdentical(const std::string& file1, const std::string& file2) 
    {
        auto fileSize1 = std::filesystem::file_size(file1);
        auto fileSize2 = std::filesystem::file_size(file2);

        const auto MAX_SIZE = 1000000; 
        if (fileSize1 > MAX_SIZE || fileSize2 > MAX_SIZE) 
        {
            return false;
        }

        std::ifstream f1(file1, std::ios::binary);
        std::ifstream f2(file2, std::ios::binary);

        if (!f1.is_open() || !f2.is_open()) 
        {
            return false;
        }

        std::vector<char> v1(std::istreambuf_iterator<char>(f1), {});
        std::vector<char> v2(std::istreambuf_iterator<char>(f2), {});

        return v1 == v2;
    }
};

TEST_F(MultithreadingCopyToolTest, CopySuccessTest) 
{
    CopyTool copyTool;
    EXPECT_EQ(copyTool.copy("source.txt", "target.txt"), ReturnCode::Success);
    EXPECT_TRUE(areFilesIdentical("source.txt", "target.txt"));
}

TEST_F(MultithreadingCopyToolTest, CopyReadErrorTest) 
{
    CopyTool copyTool;
    EXPECT_EQ(copyTool.copy("nonexistent.txt", "target.txt"), ReturnCode::ReadError);
    EXPECT_FALSE(areFilesIdentical("source.txt", "target.txt"));
}

TEST_F(MultithreadingCopyToolTest, CopyWriteErrorTest) 
{
    CopyTool copyTool;
    EXPECT_EQ(copyTool.copy("source.txt", "nonexistent_directory/target.txt"), ReturnCode::WriteError);
    EXPECT_FALSE(areFilesIdentical("source.txt", "target.txt"));
}
}