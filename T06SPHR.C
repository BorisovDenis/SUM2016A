
#include <stdlib.h>
#include <math.h>
#include "sphere.h"
#include <windows.h>
#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	CHAR *CmdLine, INT CmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpszClassName = WND_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = MyWinFunc;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindow(WND_CLASS_NAME,
		"30!",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, "Create window erroe", "ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return 30;
}

LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	static INT w, h;
	static BITMAP bm;
	static HBITMAP hBm, hBmLogo;
	static HDC hMemDC, hMemDCLogo;

	switch (Msg)
	{
	case WM_CREATE:
		SetTimer(hWnd, 30, 10, NULL);
		hDC = GetDC(hWnd);
		hMemDC = CreateCompatibleDC(hDC);
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_SIZE:
		w = LOWORD(lParam);
		h = HIWORD(lParam);
		if (hBm != NULL)
			DeleteObject(hBm);
		hDC = GetDC(hWnd);
		hBm = CreateCompatibleBitmap(hDC, w, h);
		ReleaseDC(hWnd, hDC);
		SelectObject(hMemDC, hBm);
		SendMessage(hWnd, WM_TIMER, 0, 0);
		return 0;
	case WM_ERASEBKGND:
		return;
	case WM_TIMER:
		Rectangle(hMemDC, 0, 0, w + 1, h + 1);
		srand(59);
		SetBkMode(hMemDC, TRANSPARENT);
		DrawSphere(hMemDC, w / 2, h / 2);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 30);
		DeleteDC(hMemDC);
		DeleteObject(hBm);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}