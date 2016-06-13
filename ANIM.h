#include "DEF.h"
#define VG4_MAX_UNITS 10000
typedef struct tagvg4UNIT vg4UNIT;

typedef struct
{
	HWND hWnd;
	HDC hDc;
	INT W, H;
	HBITMAP hFrame;
	vg4UNIT *Units[VG4_MAX_UNITS];
	INT NumOfUnits;

	DBL GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
	BOOL IsPause;

	INT Mx, My, Mz, Mdx, Mdy, Mdz;
	BYTE Keys[256], OldKeys[256], KeysClick[256];
	BYTE JBut[32];
	INT JPov;
	DBL JX, JY, JZ, JR;
} vg4ANIM;

struct tagvg4UNIT
{
	VOID(*Init)(vg4UNIT* Uni, vg4ANIM* Ani);
	VOID(*Close)(vg4UNIT* Uni, vg4ANIM* Ani);
	VOID(*Response)(vg4UNIT* Uni, vg4ANIM* Ani);
	VOID(*Render)(vg4UNIT* Uni, vg4ANIM* Ani);
};

extern INT VG4_MouseWheel;
extern vg4ANIM VG4_Anim;
VOID VG4_AnimInit( HWND hWnd );
VOID VG4_AnimClose( VOID );
VOID VG4_AnimResize(INT W, INT H);
VOID VG4_AnimCopyFrame(HDC hDC);
VOID VG4_AnimRender( VOID );
VOID VG4_AnimAddUnit(vg4UNIT* Uni);
VOID VG4_AnimDoExit( VOID );
VOID VG4_AnimFlipFullScreen( VOID );
vg4UNIT* VG4_AnimUnitCreate( INT Size );


