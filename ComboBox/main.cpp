#include <Windows.h>
#include <stdio.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "Combo", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); ++i)
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
			INT INDEX = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			if (INDEX != CB_ERR)
			{
				CONST INT SIZE = 256;
				CHAR g_sz_CURRENT_VALUE[SIZE] = {};
				CHAR MESSAGE[SIZE] = {};
				SendMessage(hCombo, CB_GETLBTEXT, INDEX, (LPARAM)g_sz_CURRENT_VALUE);
				sprintf_s(MESSAGE, "You selected item № %d \"%s\"", INDEX, g_sz_CURRENT_VALUE);
				MessageBox(NULL, MESSAGE, "Info", MB_OK | MB_ICONINFORMATION);
			}
			else MessageBox(NULL, "Item is not selected", "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}