#ifndef __ANIM_H_
#define __ANIM_H_
#include "DEF.h"
#define DB3_MAX_UNITS 10000
typedef struct tagdb3UNIT db3UNIT;
static MATR
DB3_RndMatrWorld;
DB3_RndMatrView;
DB3_RndMatrProj;
typedef struct
{

	HWND hWnd;
	HDC hDC;
	INT W, H;
	HBITMAP hFrame;
	db3UNIT *Units[DB3_MAX_UNITS];
	INT NumOfUnits;

	DBL GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
	BOOL IsPause;
	INT Mx, My, Mz, Mdx, Mdy, Mdz;
	BYTE Keys[256], OldKeys[256], KeysClick[256];
	BYTE JBut[32];
	INT JPov;
	DBL JX, JY, JZ, JR;
} db3ANIM;

struct tagdb3UNIT
{
	VOID(*Init)(db3UNIT *Uni, db3ANIM *Ani);
	VOID(*Close)(db3UNIT *Uni, db3ANIM *Ani);
	VOID(*Response)(db3UNIT *Uni, db3ANIM *Ani);
	VOID(*Render)(db3UNIT *Uni, db3ANIM *Ani);
};

extern INT DB3_MouseWheel;
extern db3ANIM DB3_Anim;
VOID DB3_AnimInit(HWND hWnd);
VOID DB3_AnimClose(VOID);
VOID DB3_AnimResize(INT W, INT H);
VOID DB3_AnimCopyFrame(HDC hDC);
VOID DB3_AnimRender(VOID);
VOID DB3_AnimAddUnit(db3UNIT* Uni);
VOID DB3_AnimDoExit(VOID);
VOID DB3_AnimFlipFullScreen(VOID);
db3UNIT* DB3_AnimUnitCreate(INT Size);
extern MATR
  DB3_RndMatrWorld,
  DB3_RndMatrView,
  DB3_RndMatrProj;
extern DBL
  DB3_RndProjDist,
  DB3_RndFarClip,
  DB3_RndProjSize;

#endif /* __ANIM_H_*/
