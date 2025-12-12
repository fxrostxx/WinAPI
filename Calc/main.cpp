#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG

#include <Windows.h>
#include <stdio.h>
#include <float.h>
#include "resource.h"
#ifdef DEBUG
#include <iostream>
#endif // DEBUG


CONST CHAR g_sz_WINDOW_CLASS[] = "Calc";

CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_DOUBLE_BUTTON_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;
CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 22;
CONST INT g_i_FONT_HEIGHT = g_i_DISPLAY_HEIGHT - 2;
CONST INT g_i_FONT_WIDTH = g_i_FONT_HEIGHT / 2;
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_BUTTON_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 48;

#define X_BUTTON_POSITION(position) g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (position)
#define Y_BUTTON_POSITION(position) g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (position)

CONST CHAR g_OPERATIONS[] = "+-*/";
COLORREF g_BG_COLOR = {};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR skin[]);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg = {};
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
	{
#ifdef DEBUG
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
#endif // DEBUG
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		AddFontResourceEx("Fonts\\digital-7.ttf", FR_PRIVATE, 0);
		HFONT hFont = CreateFont
		(
			g_i_FONT_HEIGHT, g_i_FONT_WIDTH,
			0, 0,
			FW_BOLD,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_TT_ALWAYS,
			ANTIALIASED_QUALITY,
			FF_DONTCARE,
			"Digital-7"
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
		CHAR sz_button[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; ++j)
			{
				sz_button[0] = i + j + '1';
				CreateWindowEx
				(
					NULL, "Button", sz_button,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					X_BUTTON_POSITION(j), Y_BUTTON_POSITION(2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					HMENU(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		HWND hButtonZero = CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			X_BUTTON_POSITION(0), Y_BUTTON_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hbmpZero = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"Buttons/Zero.bmp",
			IMAGE_BITMAP,
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButtonZero, BM_SETIMAGE, 0, (LPARAM)hbmpZero);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			X_BUTTON_POSITION(2), Y_BUTTON_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		CHAR sz_operation[2] = {};
		for (int i = 0; i < sizeof(g_OPERATIONS) / sizeof(g_OPERATIONS[0]) - 1; ++i)
		{
			sz_operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				X_BUTTON_POSITION(3), Y_BUTTON_POSITION(3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				HMENU(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			X_BUTTON_POSITION(4), Y_BUTTON_POSITION(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			X_BUTTON_POSITION(4), Y_BUTTON_POSITION(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			X_BUTTON_POSITION(4), Y_BUTTON_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		SetSkin(hwnd, "square_blue");
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, RGB(0, 66, 128));
		SetTextColor(hdc, RGB(255, 0, 0));
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
		SendMessage(hwnd, WM_ERASEBKGND, wParam, 0);
		return (LRESULT)hBrush;
	}
	break;
	case WM_COMMAND:
	{
		static DOUBLE a = DBL_MIN, b = DBL_MIN;
		static INT operation = 0;
		static BOOL input = FALSE;
		static BOOL input_operation = FALSE;
		SetFocus(hwnd);
		HWND hEdit = GetDlgItem(hwnd, IDC_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_digit[2] = {};
		CHAR sz_buffer[SIZE] = {};
		SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (input_operation)
			{
				sz_buffer[0] = {};
				input_operation = FALSE;
			}
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (!strcmp(sz_buffer, "0") || !strcmp(sz_buffer, "Error")) strcpy(sz_buffer, sz_digit);
			else lstrcat(sz_buffer, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			input = TRUE;
			input_operation = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (input_operation)
			{
				sz_buffer[0] = '0';
				sz_buffer[1] = '.';
				sz_buffer[2] = {};
				input_operation = FALSE;
			}
			if (!strchr(sz_buffer, '.'))
			{
				if (!strcmp(sz_buffer, "Error"))
				{
					strcpy(sz_buffer, "0.");
				}
				else strcat(sz_buffer, ".");
			}
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (!strcmp(sz_buffer, "Error")) strcpy(sz_buffer, "0");
			else if (!input) break;
			if (strlen(sz_buffer) == 1) sz_buffer[0] = '0';
			else sz_buffer[strlen(sz_buffer) - 1] = {};
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = input_operation = FALSE;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
			{
				if (a == DBL_MIN) a = atof(sz_buffer);
				else b = atof(sz_buffer);
				input = FALSE;
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			}
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)
			{
				if (a == DBL_MIN) a = atof(sz_buffer);
				else b = atof(sz_buffer);
				input = FALSE;
			}
			if (b == DBL_MIN) break;
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH:
				if (b) a /= b;
				else
				{
					strcpy(sz_buffer, "Error");
					a = b = DBL_MIN;
					operation = 0;
					input = input_operation = FALSE;
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
					return FALSE;
				}
				break;
			}
			input_operation = FALSE;
			sprintf(sz_buffer, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		else if (wParam >= '0' && wParam <= '9')
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		switch (wParam)
		{
		case VK_ADD:
		case VK_OEM_PLUS: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0); break;
		case VK_SUBTRACT:
		case VK_OEM_MINUS: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0); break;
		case VK_MULTIPLY: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0); break;
		case VK_DIVIDE:
		case VK_OEM_2: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0); break;
		case VK_DECIMAL:
		case VK_OEM_PERIOD: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0); break;

		case VK_BACK:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, TRUE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			break;
		case VK_ESCAPE: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, TRUE, 0); break;
		case VK_RETURN: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0); break;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - VK_NUMPAD0 + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		switch (wParam)
		{
		case VK_ADD:
		case VK_OEM_PLUS:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			break;
		case VK_SUBTRACT:
		case VK_OEM_MINUS:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0);
			break;
		case VK_MULTIPLY:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			break;
		case VK_DIVIDE:
		case VK_OEM_2:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0);
			break;
		case VK_DECIMAL:
		case VK_OEM_PERIOD:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0);
			break;

		case VK_BACK: SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0); break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0);
			break;
		case VK_RETURN:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			break;
		}
	}
	break;
	case WM_CONTEXTMENU:
	{
		HMENU cmMain = CreatePopupMenu();
		AppendMenu(cmMain, MF_STRING, IDM_SQUARE_BLUE, "Square blue");
		AppendMenu(cmMain, MF_STRING, IDM_METAL_MISTRAL, "Metal mistral");
		AppendMenu(cmMain, MF_SEPARATOR, NULL, NULL);
		AppendMenu(cmMain, MF_STRING, IDM_EXIT, "Exit");
		BOOL selected_item = TrackPopupMenu
		(
			cmMain,
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_VERNEGANIMATION,
			LOWORD(lParam), HIWORD(lParam),
			0,
			hwnd,
			NULL
		);
		switch (selected_item)
		{
		case IDM_SQUARE_BLUE: SetSkin(hwnd, "square_blue"); break;
		case IDM_METAL_MISTRAL: SetSkin(hwnd, "metal_mistral"); break;
		case IDM_EXIT: SendMessage(hwnd, WM_CLOSE, 0, 0); break;
		}
		DestroyMenu(cmMain);
	}
	break;
	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(g_BG_COLOR);
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);
	}
	break;
	case WM_DESTROY:
#ifdef DEBUG
		FreeConsole();
#endif // DEBUG
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

VOID SetSkin(HWND hwnd, CONST CHAR skin[])
{
	CHAR sz_filename[FILENAME_MAX] = {};
	for (int i = 0; i < 10; ++i)
	{
		sprintf(sz_filename, "Buttons\\%s\\button_%i.bmp", skin, i);
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
		HBITMAP bmpButton = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			i == 0 ? g_i_DOUBLE_BUTTON_SIZE : g_i_BUTTON_SIZE,
			g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)bmpButton);
	}
	CONST CHAR* button_name[] = { "point", "plus", "minus", "aster", "slash", "bsp", "clr", "equal" };
	for (int i = 0; i < 8; ++i)
	{
		sprintf(sz_filename, "Buttons\\%s\\button_%s.bmp", skin, button_name[i]);
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_POINT + i);
		HBITMAP bmpButton = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			g_i_BUTTON_SIZE,
			i == IDC_BUTTON_EQUAL - IDC_BUTTON_POINT ? g_i_DOUBLE_BUTTON_SIZE : g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)bmpButton);
	}
	if (!strcmp(skin, "square_blue")) g_BG_COLOR = RGB(0, 66, 128);
	else if (!strcmp(skin, "metal_mistral")) g_BG_COLOR = RGB(161, 157, 147);
	InvalidateRect(hwnd, NULL, TRUE);
}