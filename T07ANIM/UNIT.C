#include "anim.h"
#include <windows.h>

static VOID DB3_UnitInit(db3UNIT *Uni, db3ANIM *Ani)
{
} /* End of 'DB3_UnitInit' function */

  /* Unit deinitialization function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * db3UNIT *Uni;
  * - animation context:
  * db3ANIM *Ani;
  * RETURNS: None.
  */

static VOID DB3_UnitClose(db3UNIT *Uni, db3ANIM *Ani)
{
} /* End of 'DB3_UnitClose' function */

  /* Unit inter fdbme events handle function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * db3UNIT *Uni;
  * - animation context:
  * db3ANIM *Ani;
  * RETURNS: None.
  */

static VOID DB3_UnitResponse(db3UNIT *Uni, db3ANIM *Ani)
{
} /* End of 'db3_UnitResponse' function */

  /* Unit render function.
  * ARGUMENTS:
  * - self-pointer to unit object:
  * db3UNIT *Uni;
  * - animation context:
  * db3ANIM *Ani;
  * RETURNS: None.
  */

static VOID DB3_UnitRender(db3UNIT *Uni, db3ANIM *Ani)
{
} /* End of 'DB3_UnitRender' function */

  /* Unit creation function.
  * ARGUMENTS:
  * - unit structure size in bytes:
  * INT Size;
  * RETURNS:
  * (db34UNIT *) pointer to created unit.
  */
db3UNIT * DB3_AnimUnitCreate(INT Size)
{
	db3UNIT *Uni;

	/* Memory allocation */
	if (Size < sizeof(db3UNIT) ||
		(Uni = malloc(Size)) == NULL)
		return NULL;
	memset(Uni, 0, Size);
	/* Setup unit methods */
	Uni->Init = DB3_UnitInit;
	Uni->Close = DB3_UnitClose;
	Uni->Response = DB3_UnitResponse;
	Uni->Render = DB3_UnitRender;
	return Uni;
} /* End of 'DB3_AnimUnitCreate' function */