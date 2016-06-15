#include <stdio.h>
#include "ANIM.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#define DB3_GET_JOYSTICK_AXIS(A)\
(2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)
INT DB3_MouseWheel;
db3ANIM DB3_Anim;
static UINT64
DB3_StartTime,
DB3_OldTime,
DB3_OldTimeFPS,
DB3_PauseTime,
DB3_TimePerSec,
DB3_FrameCounter;


VOID DB3_AnimInit(HWND hWnd)
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Store window and create memory device context */
  DB3_Anim.hWnd = hWnd;
  DB3_Anim.hDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  DB3_Anim.hGLRC = wglCreateContext(DB3_Anim.hDC);
  wglMakeCurrent(DB3_Anim.hDC, DB3_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(DB3_Anim.hGLRC);
    ReleaseDC(DB3_Anim.hWnd, DB3_Anim.hDC);
    exit(0);
  }
  /* ???????????????????????????????????? */
  HDC hDC;
  LARGE_INTEGER t;
  memset(&DB3_Anim, 0, sizeof(db3ANIM));
  DB3_Anim.hWnd = hWnd; 
  hDC = GetDC(hWnd);
  DB3_Anim.hDc = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  QueryPerformanceFrequency(&t);
  DB3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  DB3_StartTime = DB3_OldTime = DB3_OldTimeFPS = t.QuadPart;
  DB3_PauseTime = 0;

  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */


}

VOID DB3_AnimClose(VOID)
{
/* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DB3_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(DB3_Anim.hWnd, DB3_Anim.hDC);

}

VOID DB3_AnimResize(INT W, INT H)
{
  /* ??????????????????????????????????????????????????????????????? */
  HDC hDC;
  DB3_Anim.W = W;
  DB3_Anim.H = H;
  if (DB3_Anim.hFrame != NULL)
    DeleteObject(DB3_Anim.hFrame);
  hDC = GetDC(DB3_Anim.hWnd);
  DB3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(DB3_Anim.hWnd, hDC);
  SelectObject(DB3_Anim.hDc, DB3_Anim.hFrame);
}

VOID DB3_AnimCopyFrame(HDC hDC)
{
  SwapBuffers(DB3_Anim.hDC);
}

VOID DB3_AnimRender(VOID)
{
  INT i;
  POINT pt;
  LARGE_INTEGER t;
  DB3_FrameCounter++;
  QueryPerformanceCounter(&t);
  DB3_Anim.GlobalTime = (DBL)(t.QuadPart - DB3_StartTime) / DB3_TimePerSec;
  DB3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - DB3_OldTime) / DB3_TimePerSec;
  if (DB3_Anim.IsPause)
  {
    DB3_Anim.DeltaTime = 0;
    DB3_PauseTime += t.QuadPart - DB3_OldTime;
  }
  else
  {
    DB3_Anim.DeltaTime = DB3_Anim.GlobalDeltaTime;
    DB3_Anim.Time = (DBL)(t.QuadPart -DB3_PauseTime - DB3_StartTime) / DB3_TimePerSec;
  }
  if (t.QuadPart - DB3_OldTimeFPS > DB3_TimePerSec)
  {
    CHAR str[100];
    DB3_Anim.FPS = DB3_FrameCounter * DB3_TimePerSec / (DBL)(t.QuadPart - DB3_OldTimeFPS);
    DB3_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5f", DB3_Anim.FPS);
    SetWindowText(DB3_Anim.hWnd, str);
    DB3_FrameCounter = 0;
  }
  DB3_OldTime = t.QuadPart;
  GetKeyboardState(DB3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DB3_Anim.Keys[i] >>= 7;
    if (!DB3_Anim.OldKeys[i] && DB3_Anim.Keys[i])
      DB3_Anim.KeysClick[i] = TRUE;
    else
      DB3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(DB3_Anim.OldKeys, DB3_Anim.Keys, 256);
  GetCursorPos(&pt);
  ScreenToClient(DB3_Anim.hWnd, &pt);
  DB3_Anim.Mdx = pt.x - DB3_Anim.Mx;
    DB3_Anim.Mdy = pt.y - DB3_Anim.My;
  DB3_Anim.Mx = pt.x;
  DB3_Anim.My = pt.y;
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;
      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        for (i = 0; i < 32; i++)
          DB3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
        DB3_Anim.JY = DB3_GET_JOYSTICK_AXIS(X);
        DB3_Anim.JX = DB3_GET_JOYSTICK_AXIS(Y);
        DB3_Anim.JZ = DB3_GET_JOYSTICK_AXIS(Z);
        DB3_Anim.JR = DB3_GET_JOYSTICK_AXIS(R);
        //DB3_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }
  for (i = 0; i < DB3_Anim.NumOfUnits; i++)
    DB3_Anim.Units[i]->Response(DB3_Anim.Units[i], &DB3_Anim);
  SelectObject(DB3_Anim.hDc, GetStockObject(NULL_PEN));
  SelectObject(DB3_Anim.hDc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(DB3_Anim.hDc, RGB(10, 155, 220));
  Rectangle(DB3_Anim.hDc, 0, 0, DB3_Anim.W + 1, DB3_Anim.H + 1);

  for (i = 0; i < DB3_Anim.NumOfUnits; i++)
  {
    SelectObject(DB3_Anim.hDc, GetStockObject(DC_PEN));
    SelectObject(DB3_Anim.hDc, GetStockObject(DC_BRUSH));
    SetDCPenColor(DB3_Anim.hDc, RGB(0, 0, 0));
    SetDCBrushColor(DB3_Anim.hDc, RGB(255, 255, 255));
    DB3_Anim.Units[i]->Render(DB3_Anim.Units[i], &DB3_Anim);
  }
  /*** Clear frame :***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*** Render all units ***/
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_RndMatrWorld = MatrIdentity();
    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }

  /* Finalize OpenGL drawing */
  glFinish();
}

VOID DB3_AnimAddUnit(db3UNIT* Uni)
{
  if (DB3_Anim.NumOfUnits < DB3_MAX_UNITS)
  {
    DB3_Anim.Units[DB3_Anim.NumOfUnits] = Uni;
    DB3_Anim.Units[DB3_Anim.NumOfUnits]->Init(DB3_Anim.Units[DB3_Anim.NumOfUnits], &DB3_Anim);
    DB3_Anim.NumOfUnits++;
  }
}

VOID DB3_AnimDoExit(VOID)
{
  static BOOL IsExit = FALSE;
  if (IsExit)
    return;
  IsExit = TRUE;
  PostMessage(DB3_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID DB3_AnimFlipFullScreen(VOID)
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
  if (IsFullScreen)
  {
    SetWindowPos(DB3_Anim.hWnd, HWND_TOP, SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;
    GetWindowRect(DB3_Anim.hWnd, &SaveRect);
    hmon = MonitorFromWindow(DB3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO*)&moninfo);
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DB3_Anim.hWnd, GWL_STYLE), FALSE);
    SetWindowPos(DB3_Anim.hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
}
/*
DB3UNIT* DB3_AnimUnitCreate(INT Size)
{
 


























































