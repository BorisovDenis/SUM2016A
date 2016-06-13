#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ANIM.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#define VG4_GET_JOYSTIC_AXIS(A)\
(2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)
INT VG4_MouseWheel;
vg4ANIM VG4_Anim;
static UINT64
VG4_StartTime,
VG4_OldTime,
VG4_OldTimeFPS,
VG4_PauseTime,
VG4_TimePerSec,
VG4_FrameCounter;


VOID VG4_AnimInit(HWND hWnd)
{
	HDC hDC;
	LARGE_INTEGER t;
	memset(&VG4_Anim, 0, sizeof(vg4ANIM));
	VG4_Anim.hWnd = hWnd;
	hDC = GetDC(hWnd);
	VG4_Anim.hDc = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);
	QueryPerformanceFrequency(&t);
	VG4_TimePerSec = t.QuadPart;
	QueryPerformanceCounter(&t);
	VG4_StartTime = VG4_OldTime = VG4_OldTimeFPS = t.QuadPart;
	VG4_PauseTime = 0;
}

VOID VG4_AnimClose(VOID)
{
	INT i;
	for (i = 0; i < VG4_Anim.NumOfUnits; i++)
	{
		VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
		free(VG4_Anim.Units[i]);
	}
	VG4_Anim.NumOfUnits = 0;
	DeleteDC(VG4_Anim.hDc);
	DeleteObject(VG4_Anim.hFrame);
	memset(&VG4_Anim, 0, sizeof(vg4ANIM));
}

VOID VG4_AnimResize(INT W, INT H)
{
	HDC hDC;
	VG4_Anim.W = W;
	VG4_Anim.H = H;
	if (VG4_Anim.hFrame != NULL)
		DeleteObject(VG4_Anim.hFrame);
	hDC = GetDC(VG4_Anim.hWnd);
	VG4_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
	ReleaseDC(VG4_Anim.hWnd, hDC);
	SelectObject(VG4_Anim.hDc, VG4_Anim.hFrame);
}

VOID VG4_AnimCopyFrame(HDC hDC)
{
	BitBlt(hDC, 0, 0, VG4_Anim.W, VG4_Anim.H, VG4_Anim.hDc, 0, 0, SRCCOPY);
}

VOID VG4_AnimRender(VOID)
{
	INT i;
	POINT pt;
	LARGE_INTEGER t;
	VG4_FrameCounter++;
	QueryPerformanceCounter(&t);
	VG4_Anim.GlobalTime = (DBL)(t.QuadPart - VG4_StartTime) / VG4_TimePerSec;
	VG4_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - VG4_OldTime) / VG4_TimePerSec;
	if (VG4_Anim.IsPause)
	{
		VG4_Anim.DeltaTime = 0;
		VG4_PauseTime += t.QuadPart - VG4_OldTime;
	}
	else
	{
		VG4_Anim.DeltaTime = VG4_Anim.GlobalDeltaTime;
		VG4_Anim.Time = (DBL)(t.QuadPart - VG4_PauseTime - VG4_StartTime) / VG4_TimePerSec;
	}
	if (t.QuadPart - VG4_OldTimeFPS > VG4_TimePerSec)
	{
		CHAR str[100];
		VG4_Anim.FPS = VG4_FrameCounter * VG4_TimePerSec / (DBL)(t.QuadPart - VG4_OldTimeFPS);
		VG4_OldTimeFPS = t.QuadPart;
		sprintf(str, "FPS: %.5f", VG4_Anim.FPS);
		SetWindowText(VG4_Anim.hWnd, str);
		VG4_FrameCounter = 0;
	}
	VG4_OldTime = t.QuadPart;
	GetKeyboardState(VG4_Anim.Keys);
	for (i = 0; i < 256; i++)
	{
		VG4_Anim.Keys[i] >>= 7;
		if (!VG4_Anim.OldKeys[i] && VG4_Anim.Keys[i])
			VG4_Anim.KeysClick[i] = TRUE;
		else
			VG4_Anim.KeysClick[i] = FALSE;
	}
	memcpy(VG4_Anim.OldKeys, VG4_Anim.Keys, 256);
	GetCursorPos(&pt);
	ScreenToClient(VG4_Anim.hWnd, &pt);
	VG4_Anim.Mdx = pt.x - VG4_Anim.Mx;
	VG4_Anim.Mdy = pt.y - VG4_Anim.My;
	VG4_Anim.Mx = pt.x;
	VG4_Anim.My = pt.y;
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
					VG4_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
				VG4_Anim.JX = VG4_GET_JOYSTIC_AXIS(X);
				VG4_Anim.JY = VG4_GET_JOYSTIC_AXIS(Y);
				VG4_Anim.JZ = VG4_GET_JOYSTIC_AXIS(Z);
				VG4_Anim.JR = VG4_GET_JOYSTIC_AXIS(R);
				VG4_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
			}
		}
	}
	for (i = 0; i < VG4_Anim.NumOfUnits; i++)
		VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);
	SelectObject(VG4_Anim.hDc, GetStockObject(NULL_PEN));
	SelectObject(VG4_Anim.hDc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(VG4_Anim.hDc, RGB(100, 155, 220));
	Rectangle(VG4_Anim.hDc, 0, 0, VG4_Anim.W + 1, VG4_Anim.H + 1);

	for (i = 0; i < VG4_Anim.NumOfUnits; i++)
	{
		SelectObject(VG4_Anim.hDc, GetStockObject(DC_PEN));
		SelectObject(VG4_Anim.hDc, GetStockObject(DC_BRUSH));
		SetDCPenColor(VG4_Anim.hDc, RGB(0, 0, 0));
		SetDCBrushColor(VG4_Anim.hDc, RGB(255, 255, 255));
		VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
	}
}

VOID VG4_AnimAddUnit(vg4UNIT* Uni)
{
	if (VG4_Anim.NumOfUnits < VG4_MAX_UNITS)
	{
		VG4_Anim.Units[VG4_Anim.NumOfUnits] = Uni;
		VG4_Anim.Units[VG4_Anim.NumOfUnits]->Init(VG4_Anim.Units[VG4_Anim.NumOfUnits], &VG4_Anim);
		VG4_Anim.NumOfUnits++;
	}
}

VOID VG4_AnimDoExit(VOID)
{
	static BOOL IsExit = FALSE;
	if (IsExit)
		return;
	IsExit = TRUE;
	PostMessage(VG4_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID VG4_AnimFlipFullScreen(VOID)
{
	static BOOL IsFullScreen = FALSE;
	static RECT SaveRect;
	if (IsFullScreen)
	{
		SetWindowPos(VG4_Anim.hWnd, HWND_TOP, SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, SWP_NOOWNERZORDER);
	}
	else
	{
		HMONITOR hmon;
		MONITORINFO moninfo;
		RECT rc;
		GetWindowRect(VG4_Anim.hWnd, &SaveRect);
		hmon = MonitorFromWindow(VG4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
		moninfo.cbSize = sizeof(moninfo);
		GetMonitorInfo(hmon, (MONITORINFO*)&moninfo);
		rc = moninfo.rcMonitor;
		AdjustWindowRect(&rc, GetWindowLong(VG4_Anim.hWnd, GWL_STYLE), FALSE);
		SetWindowPos(VG4_Anim.hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
	}
	IsFullScreen = !IsFullScreen;
}
/*
vg4UNIT* VG4_AnimUnitCreate(INT Size)
{

}*/




