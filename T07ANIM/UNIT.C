#include "ANIM.h"
static VOID DB3_UnitInit(db3UNIT* Uni, db3ANIM* Ani)
{
}

static VOID DB3_UnitClose(db3UNIT* Uni, db3ANIM* Ani)
{
}

static VOID DB3_UnitResponse(db3UNIT* Uni, db3ANIM* Ani)
{
}

static VOID DB3_UnitRender(db3UNIT* Uni, db3ANIM* Ani)
{
}

db3UNIT* DB3_AnimUnitCreate(INT Size)
{
  db3UNIT* Uni;
  if (Size < sizeof(db3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  Uni->Init = DB3_UnitInit;
  Uni->Close = DB3_UnitClose;
  Uni->Response = DB3_UnitResponse;
  Uni->Render = DB3_UnitRender;
  return Uni;
}