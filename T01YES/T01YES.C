#include <windows.h> 
#include <math.h>
#include <stdlib.h>
#define WND_CLASS_NAME "My Window Class"
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye(HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT R1)
{

  DOUBLE t;
  POINT pt;
  INT dx, dy;
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  dx = pt.x - X;
  dy = pt.y - Y;
  t = (R - R1) / sqrt(dx * dx + dy * dy);
  if (t < 1)
  {
    dx *= t;
    dy *= t;
  }
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1, Y + dy + R1 );
}
/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

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

  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;


  switch (Msg)
  {
    case WM_CREATE:
      SetTimer(hWnd, 40, 20, NULL);
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
    case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    srand(59);
    //DrawEye(hWnd, hMemDC, 100, 100, 50, 15);
    //DrawEye(hWnd, hMemDC, 300, 200, 150, 30);
    for (i = 0; i < 30; i++)
      DrawEye(hWnd, hMemDC, rand() % 800, rand() % 700, 30, 8);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 0, 0));
    TextOut(hMemDC, 30, 30, "30!", 3);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
    //case WM_TIMER:
      //Rectangle(hMemDC, 0, 0, w + 1, h + 1);
      //DrawEye(hWnd, 100, 100, 50, 15);
      //DrawEye(hWnd, 300, 20, 150, 10);
     // srand(59);
     // SetBkMode(hMemDC, TRANSPARENT);
     // SetTextColor(hMemDC, RGB(255, 0, 255));
     // TextOut(hMemDC, 30, 30, "30!", 3);
     // InvalidateRect(hWnd, NULL, FALSE);
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