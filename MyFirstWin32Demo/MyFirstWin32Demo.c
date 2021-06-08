#include <windows.h>
#include <tchar.h>
#include "resource.h"


HINSTANCE hInst;
HWND hWndMain;
RECT myRect = { 10,10,100,100 };
LRESULT MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		PAINTSTRUCT ps;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		Rectangle(ps.hdc,myRect.left,myRect.top,myRect.right,myRect.bottom);
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		myRect.right = LOWORD(lParam);
		myRect.bottom = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == 0 && LOWORD(wParam) == ID_FILE_EXIT)
			PostQuitMessage(0);
		break;
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
	if (!RegisterClassEx(&wc))
	{
		DWORD err = GetLastError();
		return 0;
	}

	hWndMain = CreateWindow(_T("MyMainWndClass"), _T("MyWindow"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL, (HMENU)NULL, hInst, (LPVOID)NULL);
	ShowWindow(hWndMain, SW_SHOWNORMAL);
	UpdateWindow(hWndMain);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}