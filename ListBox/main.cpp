#include <Windows.h>
#include <stdio.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);
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
		SetFocus(hList);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDADD:
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcAdd, 0);
		}
		break;
		case IDDELETE:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index != LB_ERR) SendMessage(hList, LB_DELETESTRING, index, 0);
		}
		break;
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (i != LB_ERR)
			{
				CONST INT SIZE = 256;
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
		case IDC_LIST1:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcEdit, 0);
		}
		break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	/*case WM_KEYUP:
		if (wParam == VK_RETURN)
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (i != LB_ERR)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcEdit, 0);
			}
			else MessageBox(hwnd, "Select item to edit", "Warning", MB_OK | MB_ICONWARNING);
		}
		break;*/
	}
	return FALSE;
}
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			if (strlen(sz_buffer))
			{
				if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				{
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
					EndDialog(hwnd, 0);
				}
				else MessageBox(hwnd, "This element already exists", "Warning", MB_OK | MB_ICONWARNING);
			}
			else MessageBox(hwnd, "Enter text", "Warning", MB_OK | MB_ICONWARNING);
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
BOOL CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменить");
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		HWND hParent = GetParent(hwnd);
		HWND hList = GetDlgItem(hParent, IDC_LIST1);
		INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
		SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		SetFocus(hEdit);
		SendMessage(hEdit, EM_SETSEL, strlen(sz_buffer), -1);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (strlen(sz_buffer))
			{
				if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				{
					SendMessage(hList, LB_DELETESTRING, i, 0);
					SendMessage(hList, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
					EndDialog(hwnd, 0);
				}
				else MessageBox(hwnd, "This element already exists", "Warning", MB_OK | MB_ICONWARNING);
			}
			else MessageBox(hwnd, "Enter text", "Warning", MB_OK | MB_ICONWARNING);
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