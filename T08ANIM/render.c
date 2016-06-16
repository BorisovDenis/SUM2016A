/* FILE NAME: RENDER.C
 * PROGRAMMER: Borisov Denis
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Global transformation matrices */
MATR
  DB3_RndMatrWorld, /* World (object) transformation matrix */
  DB3_RndMatrView,  /* Camera view transform */
  DB3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  DB3_RndProjDist, /* Near clip plane */
  DB3_RndFarClip,  /* Far clip plane */
  DB3_RndProjSize; /* Project plane size */                         
                                                                  
/* Shader support */                                                
UINT DB3_RndPrg;                         
                                                                    
/* Materials array */                                               
db3MTL DB3_RndMaterials[DB3_MAX_MATERIALS];
INT DB3_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (DB3_Anim.W >= DB3_Anim.H)
    ratio_x = (DBL)DB3_Anim.W / DB3_Anim.H;
  else
    ratio_y = (DBL)VG4_Anim.H / DB3_Anim.W;

  DB3_RndMatrProj = MatrFrustum(-ratio_x * DB3_RndProjSize / 2,
                                 ratio_x * DB3_RndProjSize / 2,
                                -ratio_y * DB3_RndProjSize / 2,
                                 ratio_y * DB3_RndProjSize / 2,
                                 DB3_RndProjDist, DB3_RndFarClip);
} /* End of 'DB3_RndSetProj' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       db3OBJ *Obj;
 * RETURNS: None.
 */
VOID DB3_RndObjDraw( vg4OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = DB3_RndMatrWorld;
    VG4_RndMatrWorld = MatrMulMatr(DB3_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(DB3_RndMatrWorld,
      MatrMulMatr(DB3_RndMatrView, DB3_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    /*
    glBegin(GL_LINES);
      glColor3d(1, 0, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(1, 0, 0, 0);
      glColor3d(0, 1, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 1, 0, 0);
      glColor3d(0, 0, 1);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 0, 1, 0);
    glEnd();
    */

    glUseProgram(DB3_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (DB3_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, DB3_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(DB3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(DB3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(DB3_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &VG4_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(DB3_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &VG4_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(DB3_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &VG4_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(DB3_RndPrg, "Ph")) != -1)
        glUniform1f(loc, VG4_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(DB3_RndPrg, "Trans")) != -1)
        glUniform1f(loc, VG4_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */
    if ((loc = glGetUniformLocation(DB3_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(VG4_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, DB3_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(VG4_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, DB3_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(DB3_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(DB3_RndPrg, "Time")) != -1)
      glUniform1f(loc, VG4_Anim.Time);
    if ((loc = glGetUniformLocation(DB3_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    DB3_RndMatrWorld = MSave;
  }
} /* End of 'VG4_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       db3OBJ *Obj;
 * RETURNS: None.
 */
VOID DB3_RndObjFree( db3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(vg4OBJ));
} /* End of 'VG4_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT DB3_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < DB3_RndNumOfMaterials; i++)
    if (strcmp(Name, DB3_RndMaterials[i].Name) == 0)  /* #include <string.h> */
      return i;
  return -1;
} /* End of 'DB3_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID VG4_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials, x, y;
  UINT t;
  DWORD Sign;
  FILE *F;
  vg4MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (DB3_RndNumOfMaterials >= DB3_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(db3MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      /* Flip image */
      /*
      for (y = 0; y < M.TexH / 2; y++)
        for (x = 0; x < M.TexW * M.TexNo; x++)
        {
          INT
            first = y * M.TexW * M.TexNo + x,
            last = (M.TexH - 1 - y) * M.TexW * M.TexNo + x;
          BYTE tmp = Image[first];

          Image[first] = Image[last];
          Image[last] = tmp;
        }
      */
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    DB3_RndMaterials[DB3_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'DB3_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */
