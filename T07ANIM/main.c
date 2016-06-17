/* FILE NAME: T07ANIM.C
 * PROGRAMMER: DB3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */


#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "anim.h"
#include "units.h"

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wPadbm, LPARAM lPadbm);

/* The main progdbm function */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	CHAR *CmdLine, INT CmdShow)

{
	INT i;
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	SetDbgMemHooks();

	/* Register window class */
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

	/* Create window */
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

	/* Show window */
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
	for (i = 0; i < 1000; i++)
		DB3_AnimAddUnit(DB3_UnitCreateBall());

	while (GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);

	return 30;
}

LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wPadbm, LPARAM lPadbm)
{
	HDC hDC;
	PAINTSTRUCT ps;
	MINMAXINFO *MinMax;
	switch (Msg)
	{
	case WM_GETMINMAXINFO:
		MinMax = (MINMAXINFO *)lPadbm;
		MinMax->ptMaxTrackSize.y =
			GetSystemMetrics(SM_CYMAXTRACK) +
			GetSystemMetrics(SM_CYCAPTION) +
			GetSystemMetrics(SM_CYMENU) +
			GetSystemMetrics(SM_CYBORDER) * 2;
		return 0;
	case WM_CREATE:
		SetTimer(hWnd, 30, 10, NULL);
		DB3_AnimInit(hWnd);
		return 0;
	case WM_SIZE:
		DB3_AnimResize(LOWORD(lPadbm), HIWORD(lPadbm));
		SendMessage(hWnd, WM_TIMER, 0, 0);
		return 0;
	case WM_MOUSEWHEEL:
		DB3_MouseWheel += (SHORT)HIWORD(wPadbm);
		return 0;
	case WM_TIMER:
		DB3_AnimRender();
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		DB3_AnimCopyFrame(hDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DB3_AnimClose();
		KillTimer(hWnd, 30);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wPadbm, lPadbm);
}