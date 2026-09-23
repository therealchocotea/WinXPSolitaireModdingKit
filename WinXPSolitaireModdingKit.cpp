#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <filesystem>

#include <imgui.h>

static std::string s_bak_path;



namespace IO
{
	bool backup(const std::string& path)
	{
		const auto s = std::filesystem::file_size(path);
		const std::string& bak_path = std::string(path + ".bak");
		std::filesystem::copy(path, bak_path);
		// Sanity checking the copy
		const auto s_bak = std::filesystem::file_size(bak_path);
		if (s == s_bak) return true;
#ifdef _DEBUG
		std::cerr << \
			"The file size of the original file and its backup \
			don't match up in size. Backup failed!"
			<< std::endl;	
#endif	
			return false;
	}
}

namespace Binary
{
	
}

int main(int argc, char argv[])
{
}