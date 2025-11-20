#include <Windows.h>
#include <stdio.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List", "Box" };
#define SIZE 256

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		HWND hList = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); ++i)
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDADD:
		{
			CHAR sz_buffer[SIZE] = {};
			HINSTANCE hInstance = GetModuleHandle(NULL);
			if (DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)InputDlgProc, (LPARAM)sz_buffer) == IDOK)
			{
				if (strlen(sz_buffer))
				{
					HWND hList = GetDlgItem(hwnd, IDC_LIST1);
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
				}
			}
		}
		break;
		case IDREMOVE:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, index, 0);
		}
		break;
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (i != LB_ERR)
			{
				CHAR sz_buffer[SIZE] = {};
				CHAR sz_message[SIZE] = {};
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
				sprintf_s(sz_message, "You selected item № %d \"%s\"", i, sz_buffer);
				MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
			}
			else MessageBox(hwnd, "Item is not selected", "Info", MB_OK | MB_ICONINFORMATION);
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
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		static CHAR* psz_buffer = (CHAR*)lParam;
		SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)psz_buffer);
			EndDialog(hwnd, IDOK);
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