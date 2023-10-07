#include "GBCodeGeneratorUtilities.h"
#ifdef _WIN32
#include <windows.h>
#elif (__linux__)
#include <linux/limits.h>
#include <unistd.h>
#elif (__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
#endif

namespace GB
{
	fs::path GetExeDirectory()
	{
#ifdef _WIN32
		// Windows specific
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);
		// TODO: Mac specific code?
#elif (__linux__)
		// Linux specific
		char exePath[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", exePath, PATH_MAX);
		if (count < 0 || count >= PATH_MAX)
			return {}; // some error
		exePath[count] = '\0';
#elif (__APPLE__)
		char exePath[PATH_MAX];
		uint32_t bufsize = PATH_MAX;
		if (!_NSGetExecutablePath(exePath, &bufsize))
			puts(exePath);
#else
		#error "Error: Undefined platform!"
#endif
		return std::filesystem::path{ exePath }.parent_path() / ""; // to finish the folder path with (back)slash
	}

	bool CreateDirectoryRecursive(const std::string& fullDirectoryPath, std::error_code& err)
	{
		err.clear();
		if (!fs::create_directories(fullDirectoryPath, err))
		{
			if (fs::exists(fullDirectoryPath))
			{
				// The folder already exists:
				err.clear();
				return true;
			}
			return false;
		}
		return true;
	}
}
