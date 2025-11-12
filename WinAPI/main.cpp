#include <Windows.h>
#include "resource.h"

//#define MESSAGE_BOX

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
		break;
	case WM_COMMAND:	// Обрабатывает комманды с клавиатуры и мыши
		switch (LOWORD(wParam))
		{
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