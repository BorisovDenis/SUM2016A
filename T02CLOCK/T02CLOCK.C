#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define WND_CLASS_NAME "My Window Class" 

VOID Arrow(HDC hDC, HWND hWnd, INT X, INT Y, INT s, INT l, DOUBLE a,  INT r,  INT g,  INT b,  INT r1,  INT g1,  INT b1) 
{ 
  INT i; 
  DOUBLE si, co; 
  POINT pt;
  POINT p[] = 
  { 
    { 0, -s },{ -s, 0 },{ 0, l } 
  }; 
  POINT p1[] = 
  { 
    { 0, -s },{ s, 0 },{ 0, l } 
  }; 
  POINT pts[sizeof(p) / sizeof(p[0])]; 
  POINT pts1[sizeof(p1) / sizeof(p1[0])]; 
  si = sin(a); 
  co = cos(a); 

  for (i = 0; i < sizeof(p) / sizeof(p[0]); i++) 
  { 
    pts[i].x = X + p[i].x * co + p[i].y * si;
    pts[i].y = Y - ( - p[i].x * si + p[i].y * co);
    pts1[i].x = X + p1[i].x * co + p1[i].y * si;
    pts1[i].y = Y - ( - p1[i].x * si + p1[i].y * co);
  } 
  SelectObject(hDC, GetStockObject(DC_PEN)); 
  SelectObject(hDC, GetStockObject(DC_BRUSH)); 
 
  srand(clock() / 1000); 
  
  SetDCPenColor(hDC, /* RGB(rand() % 256, rand() % 256, rand() % 256)*/RGB(r, g, b));
  SetDCBrushColor(hDC, /* RGB(rand() % 256, rand() % 256, rand() % 256)*/ RGB(r, g, b)); 
  Polygon(hDC, pts, sizeof(p) / sizeof(p[0])); 

  SetDCPenColor(hDC, /* RGB(rand() % 256, rand() % 256, rand() % 256) */ RGB(r1, g1, b1));
  SetDCBrushColor(hDC, /* RGB(rand() % 256, rand() % 256, rand() % 256) */ RGB(r1, g1, b1)); 
  Polygon(hDC, pts1, sizeof(p1) / sizeof(p1[0])); 

  SetDCPenColor(hDC, RGB(255, 255, 255)); 
  SetDCBrushColor(hDC, RGB(255, 255, 255)); 
} /* End of 'Draw' function */ 

/* Forward references */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
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
  CHAR s[20];
  HDC hDC;
  PAINTSTRUCT ps;
  DOUBLE p, a;
  DOUBLE r;
  SYSTEMTIME t;        
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  p = 3.14159265358979323846;
  GetLocalTime(&t);
 

  switch (Msg)
  {
    case WM_CREATE:
      SetTimer(hWnd, 30, 10, NULL);
      hBmLogo = LoadImage(NULL, "Sport.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      GetObject(hBmLogo, sizeof(bm), &bm);
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);
      hMemDCLogo = CreateCompatibleDC(hDC);
      SelectObject(hMemDCLogo, hBmLogo);
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
    case WM_ERASEBKGND: 
      return 0;
    case WM_TIMER:
      Rectangle(hMemDC, 0, 0, w + 1, h + 1);
      BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight ,hMemDCLogo, 0, 0, SRCCOPY);
      srand(59);
      SetBkMode(hMemDC, TRANSPARENT);
      SetTextColor(hMemDC, RGB(255, 0, 0));
      a = (t.wSecond + t.wMilliseconds / 1000.0) * 2 * p / 60;
      //r = bm.bmWidth / 3;
      //MoveToEx(hMemDC, w / 2, h / 2, NULL);
      //LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
      Arrow(hMemDC, hWnd,  w / 2, h / 2, 3, 220, a, 170, 0, 0, 130, 0, 0); 
      a  = (t.wMinute + t.wSecond / 60.0) * 2 * p /60;
      //r = bm.bmWidth / 5;
      Arrow(hMemDC, hWnd,  w / 2, h / 2, 5, 170, a, 0, 0, 0, 20, 20, 20); 
      //MoveToEx(hMemDC, w / 2, h / 2, NULL);
      //LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
      a  = (t.wHour + t.wMinute / 60.0) / 12 * p / 60 * 2;
      //r  = bm.bmWidth / 4;
      Arrow(hMemDC, hWnd,  w / 2, h / 2, 8, 120, a, 0, 0, 150, 0, 0, 130); 
      //MoveToEx(hMemDC, w / 2, h / 2, NULL);
      //LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
      TextOut(hMemDCLogo, 60,  60, s, sprintf(s, "%02i.%02i.%i", t.wDay, t.wMonth, t.wYear));
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
    /* End of 'MyWinFunc' function */
} /* END OF 'T02CLOCK.C' FILE */