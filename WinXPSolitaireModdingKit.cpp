#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include <windows.h>

#define APPLICATION_NAME L"WinXPSolitaireModdingKit"
#define WIN_WIDTH 500
#define WIN_HEIGHT 400
namespace
{
	constexpr wchar_t CLASS_NAME[] = APPLICATION_NAME;
	constexpr uint32_t APPLY_PATCH_INVERT_BG_COLOR = 1001;
	constexpr uint32_t WIN_CLOSE = 1002;
	RECT win_proportions;

	HWND main_win = nullptr;
}

namespace IO
{
	bool backup(HWND owner, const std::filesystem::path& path)
	{
		const std::wstring msg = L"Target does not exist";
		path.wstring();

		try {
			if (!std::filesystem::exists(path)) {
				std::cerr << "Failed to find file";
				return false;
			}

			const auto path_bak = path.string() + ".bak";
			std::filesystem::copy_file(
				path,
				path_bak,
				std::filesystem::copy_options::overwrite_existing
			);
			const auto s = std::filesystem::file_size(path);
			const auto s_bak = std::filesystem::file_size(path_bak);

			if (s != s_bak) {
				MessageBoxW(
					owner,
					msg.c_str(),
					L"Backup failed",
					MB_OK | MB_ICONERROR

				);
				return false;
			}
			return true;
		}
		catch (const std::filesystem::filesystem_error& err) {
			MessageBoxA(
				owner,
				err.what(),
				"Unexpected error",
				MB_OK | MB_ICONERROR
			);
			return false;
		}
	}
}

void apply_inv_bg_color_patch(
	HWND hwnd
	//const std::filesystem::path& path
) {
	/*if (!IO::backup(hwnd, path)) {
		return;
	}*/

	// to do

	MessageBoxW(
		hwnd,
		L"Colors inverted",
		L"Done",
		MB_OK | MB_ICONINFORMATION
	);
}

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM w_param,
	LPARAM l_param
) {
	GetWindowRect(hwnd, &win_proportions);

	auto center_x = win_proportions.right - win_proportions.left;
	auto center_y = win_proportions.bottom - win_proportions.top;

	switch (msg) {
	case WM_CREATE:
	{
		//CreateWindowW(
		//	L"STATIC",
		//	APPLICATION_NAME,
		//	WS_VISIBLE | WS_CHILD,
		//	20, 20, 300, 25,
		//	hwnd,
		//	nullptr,
		//	GetModuleHandleW(nullptr),
		//	nullptr
		//);
		CreateWindowW(
			L"STATIC",
			L"Invert background color",
			WS_VISIBLE | WS_CHILD,
			20, 20, 460, 30,
			hwnd,
			nullptr,
			GetModuleHandleW(nullptr),
			nullptr
		);
		CreateWindowW(
			L"BUTTON",
			L"Apply",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			20, 60, 100, 30,
			hwnd,
			nullptr,
			GetModuleHandleW(nullptr),
			reinterpret_cast<HMENU>(
				static_cast<INT_PTR>(APPLY_PATCH_INVERT_BG_COLOR)
				),
			GetModuleHandleW(nullptr),
			nullptr
		);

		// Perhaps I should make this more appropriately
		// a menu item and not a button, but that's
		// for later
		CreateWindowW(
			L"BUTTON",
			L"QUIT",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			360, 60, 100, 30,
			hwnd,
			reinterpret_cast<HMENU>(
				static_cast<INT_PTR>(WIN_CLOSE)
				),
			GetModuleHandleW(nullptr),
			nullptr
		);
	}
	case WM_COMMAND:
	{
		// extent here
		switch (LOWORD(w_param)) {
		case APPLY_PATCH_INVERT_BG_COLOR:
			//apply_inv_bg_color_patch(hwnd, path);
			std::cout << "Here is where the function should be" << "\n";
			return EXIT_SUCCESS;
		case WIN_CLOSE:
			DestroyWindow(hwnd);
			return EXIT_SUCCESS;
		default:
			return EXIT_SUCCESS;
		}
	}
	default:
		return DefWindowProcW(
			hwnd,
			msg,
			w_param,
			l_param
		);
	}
}

int WINAPI WinMain(
	HINSTANCE h_inst,
	HINSTANCE,
	LPSTR,
	int n_cmd_show
) {
	WNDCLASSEXW wc = {
		.cbSize = sizeof(wc),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.hInstance = h_inst,
		.hIcon = LoadIcon(
			nullptr,
			IDI_APPLICATION
		),
		.hCursor = LoadCursorW(
			nullptr,
			IDC_ARROW
		),
		.hbrBackground = reinterpret_cast<HBRUSH>(
				GetStockObject(WHITE_BRUSH)
		),
		.lpszClassName = CLASS_NAME
	};
	if (!RegisterClassEx(&wc)) {
		MessageBoxW(
			nullptr,
			L"Failed to register Window Class",
			APPLICATION_NAME,
			MB_OK | MB_ICONERROR
		);
		return EXIT_FAILURE;
	}

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		APPLICATION_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIN_WIDTH, WIN_HEIGHT,
		nullptr,
		nullptr,
		h_inst,
		nullptr
	);

	if (hwnd == nullptr) {
		MessageBoxW(
			nullptr,
			L"Failed ot create main window",
			APPLICATION_NAME,
			MB_OK | MB_ICONERROR
		);
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, n_cmd_show);
	UpdateWindow(hwnd);

	MSG msg{};

	while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	UnregisterClassW(CLASS_NAME, h_inst);
	return static_cast<int>(msg.wParam);
}