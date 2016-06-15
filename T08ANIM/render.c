 /* Vertex representation type */
#include "anim.h"
typedef struct
{
  VEC P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} db3VERTEX;

/* Primitive representation type */
typedef struct
{
  db3VERTEX *V;     /* Primitive vertex array */
  INT NumOfV;       /* Point array size */
  INT *I;           /* Facets index array */
  INT NumOfI;       /* Facets index array size */
} db3PRIM;                     
/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID DB3_RndPrimDraw( vg4PRIM *Pr )
{
  INT i;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(VG4_RndMatrWorld,
    MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {                                    
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);    
  }
  glEnd();
} /* End of 'VG4_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID DB3_RndPrimFree( db3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(db3PRIM));
} /* End of 'VG4_RndPrimFree' function */

/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL DB3_RndPrimLoad( db3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

  memset(Pr, 0, sizeof(db3PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfP; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfP times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(db3VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(db3VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1); /* Vec4Set(Rnd0(), Rnd0(), Rnd0(), 1); */
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */
/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       as5PRIM *Pr;
 * RETURNS: None.
 */
VOID DB3_RndPrimDraw( db3PRIM *Pr )
{
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(DB3_RndMatrWorld,
    MatrMulMatr(DB3_RndMatrView, DB3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(DB3_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(DB3_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DB3_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(DB3_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DB3_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(DB3_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DB3_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(DB3_RndPrg, "Time")) != -1)
    glUniform1f(loc, DB3_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'AS5_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       as5PRIM *Pr;
 * RETURNS: None.
 */
VOID AS5_RndPrimFree( as5PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(as5PRIM));
} /* End of 'AS5_RndPrimFree' function */


