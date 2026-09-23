#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include <windows.h>

#define APPLICATION_NAME "WinXPSolitaireModdingKit";

static std::string s_bak_path;

namespace UI
{
	static constexpr uint32_t ok_button = 1002;
	static enum : uint32_t {
		APPLY_PATCH_INVERT_BG_COLOR = 1,
		WIN_CLOSE = 2
	};
	LRESULT CALLBACK WndProc(HWND hwnd,
		UINT msg,
		WPARAM wp,
		LPARAM lp)
	{
		switch (msg)
		{
		case WM_CREATE:
			CreateWindowW(
				L"STATIC",
				L"Invert background color",
				WS_VISIBLE | WS_CHILD,
				20, 20, 100, 20, hwnd,
				nullptr,
				nullptr,
				nullptr
			);
			CreateWindowW(
				L"BUTTON",
				L"Apply",
				WS_VISIBLE | WS_CHILD,
				20, 20, 100, 30, hwnd,
				(HMENU)APPLY_PATCH_INVERT_BG_COLOR,
				nullptr,
				nullptr
			);
			CreateWindowW(
				L"BUTTON",
				L"Quit",
				WS_VISIBLE | WS_CHILD,
				20, 20, 100, 50, hwnd,
				(HMENU)WIN_CLOSE,
				nullptr,
				nullptr
			);
		}
	}
}
	
	void startup()
	{

	}

	void update()
	{

	}
}

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