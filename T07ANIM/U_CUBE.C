#include <time.h>
#include "units.h"
#include "RENDER.C"

/* Ball unit representation type */
typedef struct
{
	db3UNIT; /* Base unit fields */
	VEC Pos; /* Cube position */
	DBL TimerShift; /* Timer shift phase value*/
    db3PRIM Pr;
	DBL TimerSpeed; /* Timer speed value*/
} db3UNIT_CUBE;

/* Cube points */
VEC CubeP[] =
{
	{ -1, -1, 1 },
	{ 1, -1, 1 },
	{ 1, -1, -1 },
	{ -1, -1, -1 },
	{ -1, 1, 1 },
	{ 1, 1, 1 },
	{ 1, 1, -1 },
	{ -1, 1, -1 }
};

/* Cube edges */
INT CubeE[][2] =
{
	{ 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },
	{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },
	{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }
};

/* Cube primitive */
db3PRIM Cube =
{
	CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
	CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
* ARGUMENTS:
* - self-pointer to unit object:
* db3UNIT_CUBE *Uni;
* - animation context:
* db3ANIM *Ani;
* RETURNS: None.
*/
static VOID DB3_UnitInit(db3UNIT_CUBE *Uni, db3ANIM *Ani)
{
	Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
	Uni->TimerShift = Rnd1() * 59;
	Uni->TimerSpeed = Rnd1() * 8;
	DB3_RndPrimLoad(&Uni->Pr, "g3d\\cow.g3d");
} /* End of 'DB3_UnitInit' function */

  /* Unit cube deinitialization function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * db3UNIT_CUBE *Uni;
  * - animation context:
  * db3ANIM *Ani;
  * RETURNS: None.
  */
static VOID DB3_UnitClose(db3UNIT_CUBE *Uni, db3ANIM *Ani)
{
	DB3_RndPrimFree(&Uni->Pr);
} /* End of 'DB3_UnitClose' function */

  /* Unit cube inter fdbme events handle function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * db3UNIT_CUBE *Uni;
  * - animation context:
  * db3ANIM *Ani;
  * RETURNS: None.
  */
static VOID DB3_UnitResponse(db3UNIT_CUBE *Uni, db3ANIM *Ani)
{
	if (Ani->KeysClick[VK_SPACE])
		DB3_AnimAddUnit(DB3_UnitCreateBall());
	if (Ani->KeysClick[VK_F1])
		DB3_AnimAddUnit(DB3_UnitCreateCube());
	if (Ani->KeysClick[VK_F2])
		DB3_AnimFlipFullScreen();
	if (Ani->KeysClick[VK_ESCAPE])
		DB3_AnimDoExit();
} /* End of 'DB3_UnitResponse' function */

  /* Unit render function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * DB3UNIT_CUBE *Uni;
  * - animation context:
  * DB3ANIM *Ani;
  * RETURNS: None.
  */
static VOID DB3_UnitRender(db3UNIT_CUBE *Uni, db3ANIM *Ani)
{
	DB3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.1F, 0.1F, 0.1F)),
		MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
			MatrTdbnslate(VecAddVec(Uni->Pos,
				VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
	DB3_RndPrimDdbw(&Uni->Pr);
} /* End of 'DB3_UnitRender' function */

  /* Unit cube creation function.
  * ARGUMENTS: None.
  * RETURNS:
  * (DB3UNIT *) pointer to created unit.
  */
db3UNIT * DB3_UnitCreateCube(VOID)
{
	db3UNIT_CUBE *Uni;

	if ((Uni = (db3UNIT_CUBE *)DB3_AnimUnitCreate(sizeof(db3UNIT_CUBE))) == NULL)
		return NULL;
	/* Setup unit methods */
	Uni->Init = (VOID *)DB3_UnitInit;
	Uni->Close = (VOID *)DB3_UnitClose;
	Uni->Response = (VOID *)DB3_UnitResponse;
	Uni->Render = (VOID *)DB3_UnitRender;
	return (db3UNIT *)Uni;
} /* End of 'DB3_UnitCreateCube' function */

  /* END OF 'U_CUBE.C' FILE */
