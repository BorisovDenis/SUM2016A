/* FILE NAME: RENDER.C
 * PROGRAMMER: DB3
 * DATE: 08.06.2016
 * PURPOSE: RENDER function..
 */
#include "anim.h"
VOID DB3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;
  
  if (DB3_Anim.W >= DB3_Anim.H)
    ratio_x = (DBL)DB3_Anim.W / DB3_Anim.H;
  else 
    ratio_y = (DBL)DB3_Anim.H / DB3_Anim.W;

  DB3_RndMatrProj = MatrFrustum(-ratio_x * DB3_RndProjSize / 2,
                                 ratio_x * DB3_RndProjSize / 2,
                                -ratio_y * DB3_RndProjSize / 2, 
                                 ratio_y * DB3_RndProjSize / 2,
                                 DB3_RndProjDist, DB3_RndFarClip);

}/*End of 'DB3_RndSetProj' function'*/
typedef struct
{
  VEC *P;
  INT NumOfP;
  INT (*Edges)[2];
  INT NumOfE;
}db3PRIM;

VEC CubeP[] = 
{
  {-1, -1, 1},
  {1, -1,  1},
  {1, -1, -1},
  {-1, -1,-1},
  {-1, 1,  1},
  {1,  1,  1},
  {1,  1, -1},
  {-1, 1, -1}
};
INT CibeE[][2] = 
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};
/* vg4PRIM Cube = 
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0]),
}; */
VOID DB3_RndPrimDraw( db3PRIM *Pr )
{
  INT i;
  MATR M;
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(DB3_RndMatrWorld,
    MatrMulMatr(DB3_RndMatrView, DB3_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = PointTransformProj(Pr->P[i], M);

    pts[i].x = (p.X + 1) * DB3_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * DB3_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(DB3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(DB3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'VG4_RndPrimDraw' function */

