#include <Windows.h>
#include "resource.h"

//#define MESSAGE_BOX

CONST CHAR sz_login_placeholder[] = "Enter login";
CONST CHAR sz_password_placeholder[] = "Enter password";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lPCmdLine, INT nCmdShow)
{
#ifdef MESSAGE_BOX
	MessageBox
	(
		NULL,
		"Hello WinAPI!",
		"MesseageBox",
		MB_YESNOCANCEL | MB_ICONINFORMATION | MB_HELP | MB_DEFBUTTON2 | MB_SYSTEMMODAL // MB_ - MessageBox
	);
#endif // MESSAGE_BOX
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	// Выполняется 1 раз при запуске окна
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		//SetFocus(GetDlgItem(hwnd, ID_EDIT_LOGIN));
		HWND hEditLogin = GetDlgItem(hwnd, ID_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, ID_EDIT_PASSWORD);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)sz_login_placeholder);
		SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_password_placeholder);
	}
	break;
	case WM_COMMAND:	// Обрабатывает комманды с клавиатуры и мыши
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, ID_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, ID_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case ID_EDIT_LOGIN:
		{
			HWND hEditLogin = GetDlgItem(hwnd, ID_EDIT_LOGIN);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS && !strcmp(sz_buffer, sz_login_placeholder))
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
			else if (HIWORD(wParam) == EN_KILLFOCUS && !strlen(sz_buffer))
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)sz_login_placeholder);
		}
		break;
		case ID_EDIT_PASSWORD:
		{
			HWND hEditPassword = GetDlgItem(hwnd, ID_EDIT_PASSWORD);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditPassword, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS && !strcmp(sz_buffer, sz_password_placeholder))
				SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)"");
			else if (HIWORD(wParam) == EN_KILLFOCUS && !strlen(sz_buffer))
				SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_password_placeholder);
		}
		break;
		case IDOK:
			MessageBox(NULL, "OK button was pressed", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:		// Выполняется при нажатии кнопки закрытия окна
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}