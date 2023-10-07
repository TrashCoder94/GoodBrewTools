#pragma once

#include <array>
#include <cassert>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace GB
{
	fs::path GetExeDirectory();
    bool CreateDirectoryRecursive(const std::string& fullDirectoryPath, std::error_code& err);
}