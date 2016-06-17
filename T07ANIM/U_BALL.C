#include "TIME.H"
#include "anim.h"

typedef struct
{
	db3UNIT;
	VEC Pos;
	VEC Shift;
	DBL TimerShift;
	DBL TimerSpeed;
	COLORREF Color;
} db3UNIT_BALL;

static VOID DB3_UnitInit(db3UNIT_BALL *Uni, db3ANIM *Ani)
{
	Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
	Uni->TimerShift = Rnd1() * 59;
	Uni->TimerSpeed = Rnd1() * 8;
	Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);
}

static VOID DB3_UnitResponse(db3UNIT_BALL *Uni, db3ANIM *Ani)
{
	DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;
	Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
}

static VOID DB3_UnitRender(db3UNIT_BALL *Uni, db3ANIM *Ani)
{
	VEC p = VecAddVec(Uni->Pos, Uni->Shift);
	SetDCBrushColor(Ani->hDC, RGB(255, 0, 0));
	Ellipse(Ani->hDC, p.X - 8, p.Y - 8, p.X + 8, p.Y + 8);
}

db3UNIT * DB3_UnitCreateBall(VOID)
{
	db3UNIT_BALL *Uni;

	if ((Uni = (db3UNIT_BALL *)DB3_AnimUnitCreate(sizeof(db3UNIT_BALL))) == NULL)
		return NULL;
	Uni->Init = (VOID *)DB3_UnitInit;
	Uni->Response = (VOID *)DB3_UnitResponse;
	Uni->Render = (VOID *)DB3_UnitRender;
	return (db3UNIT *)Uni;
}
