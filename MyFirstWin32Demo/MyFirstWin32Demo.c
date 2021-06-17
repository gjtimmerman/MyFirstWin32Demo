#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <math.h>


HINSTANCE hInst;
HWND hWndMain;

INT_PTR MyDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int cmd = LOWORD(wParam);
	switch (msg)
	{
	case WM_COMMAND:
		if (cmd == IDC_BUTTON1)
		{
			TCHAR buffer[80];
			GetDlgItemText(hWnd, IDC_EDIT1, buffer, 80);
			TCHAR caption[80];
			LoadString(NULL, IDS_STRING103, caption, 80);
			MessageBox(hWnd, buffer, caption, MB_OK);
			return TRUE;
		}
		else if (cmd == IDC_BUTTON2)
			EndDialog(hWnd, IDOK);
		break;
	}
	return FALSE;
}


RECT myRect = { 10,10,100,100 };
LRESULT MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		int x;
		int y;
		HMENU hMenu1;
		HMENU hMenu2;
		PAINTSTRUCT ps;
		RECT clientRect;
		HGDIOBJ prevPen;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
//		Rectangle(ps.hdc, myRect.left, myRect.top, myRect.right, myRect.bottom);
		SetMapMode(ps.hdc, MM_ANISOTROPIC);
		GetClientRect(hWnd, &clientRect);
		SetWindowOrgEx(ps.hdc, -500, -500, NULL);
		SetWindowExtEx(ps.hdc, 1000, 1000, NULL);
		SetViewportExtEx(ps.hdc, clientRect.right, clientRect.bottom, NULL);
		prevPen = SelectObject(ps.hdc, GetStockObject(BLACK_PEN));
		MoveToEx(ps.hdc,-500, 0, NULL);
		LineTo(ps.hdc, 500, 0);
		MoveToEx(ps.hdc, 0, -500, NULL);
		LineTo(ps.hdc, 0, 500);
		for (int d = -500; d < 500; d++)
			SetPixel(ps.hdc, d, -(int)(sin(d/50.0) * 50), RGB(255, 0, 0));
		SelectObject(ps.hdc, prevPen);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		myRect.right = LOWORD(lParam);
		myRect.bottom = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CONTEXTMENU:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		hMenu1 = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU2));
		hMenu2 = GetSubMenu(hMenu1,0 );
		TrackPopupMenu(hMenu2,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON, x, y, 0, hWnd, NULL);
		break;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
	{
		int source = HIWORD(wParam);
		int item = LOWORD(wParam);
		if ((source == 0 || source == 1) && LOWORD(wParam))
		{
			if (item == ID_FILE_EXIT)
				PostQuitMessage(0);
			else if (item == ID_FILE_OPEN)
			{
				TCHAR fname[180] = { 0 };
				OPENFILENAME ofn;
				memset(&ofn, 0, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.hInstance = NULL;
				ofn.lpstrFile = fname;
				ofn.nMaxFile = 180;
				if (GetOpenFileName(&ofn))
				{
					LPTSTR filename = ofn.lpstrFile;
					HDC myClientDC = GetDC(hWnd);
					TextOut(myClientDC, myRect.left + 10, myRect.top + 10, filename,(int)_tcslen(filename));
					ReleaseDC(hWnd, myClientDC);
				}
				else
				{
					DWORD error = CommDlgExtendedError();
					DWORD x = error;
				}
			}
			else if (item == ID_HELP_ABOUT)
			{
				DialogBox(NULL, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, MyDlgProc);
			}
		}
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR cmdLine, int cmdShow)
{
	hInst = hInstance;
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MyWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = _T("MyMainWndClass");
	ATOM registeredClass;
	if (!(registeredClass=RegisterClassEx(&wc)))
	{
		DWORD err = GetLastError();
		return 0;
	}

	hWndMain = CreateWindow(MAKEINTATOM(registeredClass), _T("MyWindow"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL, (HMENU)NULL, hInst, (LPVOID)NULL);
	ShowWindow(hWndMain, SW_SHOWNORMAL);
	UpdateWindow(hWndMain);
	HACCEL acceleratorTable = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWndMain, acceleratorTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}