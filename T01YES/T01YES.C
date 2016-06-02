#include <windows.h>     
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawEye(hWND, INT X, INT Y, INT R, INT R1)
{
  POINT pt;
  INT dx, dy;
  GetCursorPos(&pt);
  ScreenToClient(hWND, &PY);
  dx = pt.x - X;
  dy = pt.y - Y
  t = (R - R1) / sqrt(dx * dx + dy * dy);
  if (t < 1)
  {
    dx *= dt;
    dy *= f;
  }
  Ellipse(hDC, X - R, X + R, Y + R);
  Ellipse(hDC, x + dx - R1, Y + dy - R1, X + dx + R1, Y + dy + R1 )
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
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
  wc.lpszClassName = "My Window Class";
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;
  
  
  
  RegisterClass(&wc);


  hWnd = 
    CreateWindow("My Window Class",
      "30!",
      WS_OVERLAPPEDWINDOW | WS_BORDER | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, 
      NULL, 
      hInstance,
      NULL);  
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 0;
}
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  static INT w, h;


  switch (Msg)
  {
    case WM_CREATE:
      SerTimer(hwnd, 40, 20, NULL);
      hDC = GetDC;
      hMeMDC = CreateCompatible(hDC);
    case WM_SIZE:
      w = LOWORD(lParam);
      h = HIWORD(lParam); 
      if (hBM != NULL)
        DeleteObject(hBM);
      hDC = GetDC(hWnd);
      hBm = CreateCompatibleBitmap(hDC, w, h);
      ReleaseDC(hWnd, hDC);
      SelectObject(hMemDC, hBM);
      SendMessage(hWnd, WM_Timer, 0, 0);
      return 0;
      break;
    case WM_MOUSEMOVE:
      DrawEye(hWnd, 100, 100, 50, 15);
      DrawEye(hWnd, 300, 20, 150, 10);
      InvalidateRect(hWnd, NULL, FALSE);
      return 0;
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
     /* GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);
      Ellipse(hDC, 0, 0, 400, 500);
      Ellipse(hDC, 400, 0, 800, 500);
      Ellipse(hDC, 100, 130, 300, 350 );
      Ellipse(hDC, pt.y + 100, pt.x + 100, pt.x - 100, pt.y - 100);
      MoveToEx(hDC, 400, 200, NULL);
      LineTo(hDC, pt.x, pt.y); */
      DrawEye(hWnd, 100, 100, 50, 15);
      DrawEye(hWnd, 300, 20, 150, 10);
      EndPaint(hWnd, &ps);
      return 0;
    case WM_DESTROY:
      KillTimer(hWnd, 30);
      PostMessage(hWnd, WM_QUIT, 0, 0);
      DeleteObject(hBm);
      DeleteObject(hMemDC);
      SendQuitMessage(0);
      return 0;
    case WM_Timer:
      DrawEye(hWnd, 100, 100, 50, 15);
      DrawEye(hWnd, 300, 20, 150, 10);
      srand(59);
      SetBkMode(hMemDc, TRANSPARRENT);
      SetTextColor(hMemDC, RGB(255, 0, 255));
      TextOut(hMemDC, 30, 30, "30!", 3);
      InvalideRect(hWnd, NULL, FALSE);
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
} 