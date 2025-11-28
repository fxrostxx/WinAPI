#include <Windows.h>
#include <string>
#include "resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My first window";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID UpdateWindowTitle(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PALM));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_BITCOIN));
	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
	//wClass.hIcon = (HICON)LoadImage(NULL, "palm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hIconSm = (HICON)LoadImage(NULL, "bitcoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hCursor = (HCURSOR)LoadImage(hInstance, "opera_cursor.cur", IMAGE_CURSOR, 30, 30, LR_LOADFROMFILE);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;
	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	RECT screenRect;
	CONST DOUBLE scale = 0.75;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	INT screenWidth = screenRect.right - screenRect.left;
	INT screenHeight = screenRect.bottom - screenRect.top;
	INT windowWidth = screenWidth * scale;
	INT windowHeight = screenHeight * scale;
	INT windowX = (screenWidth - windowWidth) / 2;
	INT windowY = (screenHeight - windowHeight) / 2;

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY,
		windowWidth, windowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		UpdateWindowTitle(hwnd);
		break;
	case WM_COMMAND:
		break;
	case WM_MOVE:
		UpdateWindowTitle(hwnd);
		break;
	case WM_SIZE:
		UpdateWindowTitle(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return FALSE;
}
VOID UpdateWindowTitle(HWND hwnd)
{
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	std::string windowTitle =
		"Position: " + std::to_string(windowRect.left) + "x" + std::to_string(windowRect.top) +
		" | Size: " + std::to_string(windowRect.right - windowRect.left) + "x" + std::to_string(windowRect.bottom - windowRect.top);
	SetWindowText(hwnd, windowTitle.c_str());
}