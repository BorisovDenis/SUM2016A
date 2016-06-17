/* FILE NAME: RENDER.C
 * PROGRAMMER: DB3
 * DATE: 08.06.2016
 * PURPOSE: RENDER function..
 */
#include "anim.h"
typedef struct
{
	VEC *P;
	INT NumOfP;
	INT(*Edges)[2];
	INT NumOfE;
}db3PRIM;

/* Global tdbnsformation matrices */
MATR
DB3_RndMatrWorld, /* World (object) tdbnsformation matrix */
DB3_RndMatrView, /* Camedb view tdbnsform */
DB3_RndMatrProj; /* Projection tdbnsform */

				 /* Projection padbmeters */
DBL
DB3_RndProjDist, /* Near clip plane */
DB3_RndFarClip, /* Far clip plane */
DB3_RndProjSize; /* Project plane size */

				 /* Setup projection function.
				 * ARGUMENTS: None.
				 * RETURNS: None.
				 */
VOID DB3_RndSetProj(VOID)
{
	DBL dbtio_x = 1, dbtio_y = 1;

	if (DB3_Anim.W >= DB3_Anim.H)
		dbtio_x = (DBL)DB3_Anim.W / DB3_Anim.H;
	else
		dbtio_y = (DBL)DB3_Anim.H / DB3_Anim.W;

	DB3_RndMatrProj = MatrFrustum(-dbtio_x * DB3_RndProjSize / 2,
		dbtio_x * DB3_RndProjSize / 2,
		-dbtio_y * DB3_RndProjSize / 2,
		dbtio_y * DB3_RndProjSize / 2,
		DB3_RndProjDist, DB3_RndFarClip);
} /* End of 'DB3_RndSetProj' function */

VOID DB3_RndPrimDraw(db3PRIM *Pr)
{
	INT i;
	MATR M;
	POINT *pts;

	/* Build tdbnsform matrix */
	M = MatrMulMatr(DB3_RndMatrWorld,
		MatrMulMatr(DB3_RndMatrView, DB3_RndMatrProj));

	/* Tdbnsform all points */
	pts = malloc(sizeof(POINT) * Pr->NumOfP);
	if (pts == NULL)
		return;
	for (i = 0; i < Pr->NumOfP; i++)
	{
		/* NDC */
		VEC p = VecMulMatr(Pr->P[i], M);
		pts[i].x = (p.X + 1) * DB3_Anim.W / 2;
		pts[i].y = (-p.Y + 1) * DB3_Anim.H / 2;
	}
	/* Ddbw all lines */
	for (i = 0; i < Pr->NumOfE; i++)
	{
		INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

		MoveToEx(DB3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
		LineTo(DB3_Anim.hDC, pts[n1].x, pts[n1].y);
	}
	free(pts);
}
/* END OF 'RENDER.C' FILE */
