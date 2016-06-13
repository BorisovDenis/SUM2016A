#include <time.h>
#include "UNITS.h"

typedef struct
{
	vg4UNIT;
	VEC Pos;
	VEC Shift;
	DBL TimerShift;
	DBL TimerSpeed;
	COLORREF Color;
}vg4UNIT_BALL;

static VOID VG4_UnitInit(vg4UNIT_BALL *Uni, vg4ANIM *Ani)
{
	srand(time(NULL));
	Uni->Pos = VecSet(rand() % 1024, rand() % 768, 0);
	Uni->TimerShift = rand() % 59;
	Uni->TimerSpeed = rand() % 8;
	Uni->Color = RGB(rand() % 255, rand() % 255, rand() % 255);
}
static VOID VG4_UnitResponse(vg4UNIT_BALL *Uni, vg4ANIM *Ani)
{
	DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;

	Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
}
static VOID VG4_UnitRender(vg4UNIT_BALL *Uni, vg4ANIM *Ani)
{
	VEC p = VecAddVec(Uni->Pos, Uni->Shift);
	SetDCBrushColor(Ani->hDc, Uni->Color);
	Ellipse(Ani->hDc, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);
}

vg4UNIT* VG4_UnitCreateBall(VOID)
{
	vg4UNIT_BALL* Uni;
	if ((Uni = (vg4UNIT_BALL*)VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
		return NULL;
	((vg4UNIT*)Uni)->Init = (VOID*)VG4_UnitInit;
	((vg4UNIT*)Uni)->Response = (VOID*)VG4_UnitResponse;
	((vg4UNIT*)Uni)->Render = (VOID*)VG4_UnitRender;
	return (vg4UNIT*)Uni;
}




