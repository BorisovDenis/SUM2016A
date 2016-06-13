/* FILE NAME: T07ANIM.C
 * PROGRAMMER: DB3
 * DATE: 08.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */


/* #define _CRT_SECURE_NO_WARNINGS */
#include <windows.h>
#include "units.h"
#define DB3_WND_CLASS_NAME "My window class"
LRESULT CALLBACK DB3_MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR* CmdLine, INT ShowCmd)
{
  int i;
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  SetDbgMemHooks();
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = DB3_MyWindowFunc;
  wc.lpszClassName = DB3_WND_CLASS_NAME;
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  hWnd = CreateWindow(DB3_WND_CLASS_NAME, "Animation System", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  for (i = 0; i < 10; i++)
    DB3_AnimAddUnit(DB3_UnitCreateBall());
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
}

LRESULT CALLBACK DB3_MyWindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO* MinMax;
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO*)lParam;
    MinMax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
                               GetSystemMetrics(SM_CYCAPTION) +
                               GetSystemMetrics(SM_CYMENU) +
                               GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 11, 2, NULL);
    DB3_AnimInit(hWnd); 
    return 0;
  case WM_SIZE:
    DB3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    DB3_MouseWheel += (SHORT)HIWORD(wParam);
    DB3_AnimAddUnit(DB3_UnitCreateBall());
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    DB3_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DB3_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    DB3_AnimClose();
    KillTimer(hWnd, 11);
    PostQuitMessage(0);
    return 0; 
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}