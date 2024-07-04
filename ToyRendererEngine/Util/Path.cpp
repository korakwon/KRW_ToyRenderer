#include "Path.h"

std::string Util::Path::ReadFile(const std::string& FileName)
{
    std::ifstream File(FileName, std::ios::ate | std::ios::binary);
    assert(File.is_open() == true);

    size_t FileSize = File.tellg();
    std::string Result;
    Result.resize(FileSize);

    File.seekg(0);
    File.read(Result.data(), FileSize);

    File.close();

    return Result;
}