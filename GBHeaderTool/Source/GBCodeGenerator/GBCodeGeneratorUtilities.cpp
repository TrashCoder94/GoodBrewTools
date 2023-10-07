#include "GBCodeGeneratorUtilities.h"
#ifdef _WIN32
#include <windows.h>
#elif (__linux__)
#include <linux/limits.h>
#include <unistd.h>
#else

#endif

namespace GB
{
	fs::path GetExeDirectory()
	{
#ifdef _WIN32
		// Windows specific
		wchar_t szPath[MAX_PATH];
		GetModuleFileNameW(NULL, szPath, MAX_PATH);
		// TODO: Mac specific code?
#else
		// Linux specific
		char szPath[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
		if (count < 0 || count >= PATH_MAX)
			return {}; // some error
		szPath[count] = '\0';
#endif
		return std::filesystem::path{ szPath }.parent_path() / ""; // to finish the folder path with (back)slash
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
