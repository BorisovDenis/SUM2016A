#include "ANIM.h"
static VOID VG4_UnitInit(vg4UNIT* Uni, vg4ANIM* Ani)
{

}

static VOID VG4_UnitClose(vg4UNIT* Uni, vg4ANIM* Ani)
{

}

static VOID VG4_UnitResponse(vg4UNIT* Uni, vg4ANIM* Ani)
{

}

static VOID VG4_UnitRender(vg4UNIT* Uni, vg4ANIM* Ani)
{

}

vg4UNIT* VG4_AnimUnitCreate(INT Size)
{
	vg4UNIT* Uni;
	if (Size < sizeof(vg4UNIT) || (Uni = malloc(Size)) == NULL)
		return NULL;
	memset(Uni, 0, Size);
	Uni->Init = VG4_UnitInit;
	Uni->Close = VG4_UnitClose;
	Uni->Response = VG4_UnitResponse;
	Uni->Render = VG4_UnitRender;
	return Uni;
}