#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#define DB3_GET_JOYSTICK_AXIS(A) \
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
	HDC hDC;
	LARGE_INTEGER t;

	memset(&DB3_Anim, 0, sizeof(db3ANIM));
	DB3_Anim.hWnd = hWnd;
	hDC = GetDC(hWnd);
	DB3_Anim.hDC = CreateCompatibleDC(hDC);
	QueryPerformanceFrequency(&t);
	DB3_TimePerSec = t.QuadPart;
	QueryPerformanceCounter(&t);
	DB3_StartTime = DB3_OldTime = DB3_OldTimeFPS = t.QuadPart;
	DB3_PauseTime = 0;
}

VOID DB3_AnimClose(VOID)
{
	INT i;
	for (i = 0; i < DB3_Anim.NumOfUnits; i++)
	{
		DB3_Anim.Units[i]->Close(DB3_Anim.Units[i], &DB3_Anim);
		free(DB3_Anim.Units[i]);
	}
	DB3_Anim.NumOfUnits = 0;
	DeleteDC(DB3_Anim.hDC);
	DeleteObject(DB3_Anim.hFrame);
	memset(&DB3_Anim, 0, sizeof(db3ANIM));
}

VOID DB3_AnimResize(INT w, INT h)
{
	HDC hDC;
	DB3_Anim.W = w;
	DB3_Anim.H = h;
	if (DB3_Anim.hFrame != NULL)
		DeleteObject(DB3_Anim.hFrame);
	hDC = GetDC(DB3_Anim.hWnd);
	DB3_Anim.hFrame = CreateCompatibleBitmap(hDC, w, h);
	ReleaseDC(DB3_Anim.hWnd, hDC);
	SelectObject(DB3_Anim.hDC, DB3_Anim.hFrame);
}

VOID DB3_AnimCopyFdbme(HDC hDC)
{
	BitBlt(hDC, 0, 0, DB3_Anim.W, DB3_Anim.H, DB3_Anim.hDC, 0, 0, SRCCOPY);
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
		DB3_Anim.Time += (t.QuadPart - DB3_PauseTime - DB3_StartTime) / DB3_TimePerSec;
	}
	if (t.QuadPart - DB3_OldTimeFPS > DB3_TimePerSec)
	{
		CHAR str[100];
		DB3_Anim.FPS = DB3_FrameCounter * DB3_TimePerSec / (DBL)(t.QuadPart - DB3_OldTimeFPS);
		DB3_OldTimeFPS = t.QuadPart;
		sprintf_s(str, "FPS:%.5f", DB3_Anim.FPS);
		SetWindowText(DB3_Anim.hWnd, str);
		DB3_FrameCounter = 0;
	}
	DB3_OldTime = t.QuadPart;
	/* Keyboard */
	GetKeyboardState(DB3_Anim.Keys);
	for (i = 0; i < 256; i++)
	{
		DB3_Anim.Keys[i] >>= 7;
		if (!OldKeys[i] && DB3_Anim.Keys[i])
			DB3_Anim.KeysClick[i] = TRUE;
		else
			DB3_Anim.KeysClick[i] = FALSE;
	}
	memcpy(DB3_Anim.OldKeys, DB3_Anim.Keys, 256);
	/* Mouse */
	GetCursorPos(&pt);
	ScreenToClient(DB3_Anim.hWnd, &pt);
	DB3_Anim.mdx = pt.x - DB3_Anim.mx;
	DB3_Anim.mdy = pt.y - DB3_Anim.my;
	DB3_Anim.mx = pt.x;
	DB3_Anim.my = pt.y;
	/* JoyStick */
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
				DB3_Anim.JX = DB3_GET_JOYSTICK_AXIS(X);
				DB3_Anim.JY = DB3_GET_JOYSTICK_AXIS(Y);
				DB3_Anim.JZ = DB3_GET_JOYSTICK_AXIS(Z);
				DB3_Anim.JR = DB3_GET_JOYSTICK_AXIS(R);
			}
		}
	}
	for (i = 0; i < DB3_Anim.NumOfUnits; i++)
		DB3_Anim.Units[i]->Response(DB3_Anim.Units[i], &DB3_Anim);
	SelectObject(DB3_Anim.hDC, GetStockObject(NULL_PEN));
	SelectObject(DB3_Anim.hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(DB3_Anim.hDC, RGB(50, 100, 150));
	Rectangle(DB3_Anim.hDC, 0, 0, DB3_Anim.w + 1, DB3_Anim.h + 1);
	for (i = 0; i < DB3_Anim.NumOfUnits; i++)
	{
		SelectObject(DB3_Anim.hDC, GetStockObject(DC_PEN));
		SelectObject(DB3_Anim.hDC, GetStockObject(DC_BRUSH));
		SetDCPenColor(DB3_Anim.hDC, RGB(0, 0, 0));
		SetDCBrushColor(DB3_Anim.hDC, RGB(210, 210, 210));
		DB3_Anim.Units[i]->Render(DB3_Anim.Units[i], &DB3_Anim);
	}
}

VOID DB3_AnimAddUnit(db3UNIT *Uni)
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

VOID DB3AnimFlipFullScreen(VOID)
{
	static BOOL IsFullScreen = FALSE;
	static RECT SaveRect;
	if (IsFullScreen)
	{
		SetWindowPos(DB3_Anim.hWnd, HWND_TOP,
			SaveRect.left, SaveRect.top,
			SaveRect.right, SaveRect.bottom - SaveRect.top,
			SWP_NOOWNERZORDER);
	}
	else
	{
		HMONITOR hmon;
		MONITORINFOEX moninfo;
		RECT rc;

		GetWindowRect(DB3_Anim.hWnd, &SaveRect);
		hmon = MonitorFromWindow(DB3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
		moninfo.cbSize = sizeof(moninfo);
		GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
		rc = moninfo.rcMonitor;
		AdjustWindowRect(&rc, GetWindowLong(DB3_Anim.hWnd, GWL_STYLE), FALSE);
		SetWindowPos(DB3_Anim.hWnd, HWND_TOPMOST,
			rc.left, rc.top, rc.right, rc.bottom - rc.top,
			SWP_NOOWNERZORDER);
	}
	IsFullScreen = !IsFullScreen;
}
