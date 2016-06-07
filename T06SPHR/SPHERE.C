/* FILE NAME: SPHERE.C
 * PROGRAMMER: Borisov Denis 
 * DATE: 07.06.2016
 */
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "sphere.h"
#define pi 3.14159265358979323846  
#define N 30
#define M 15
VOID DrawSphere(HDC hDC, INT xc, INT yc)
{
  INT i, j, r = 100;
  DOUBLE theta, phi, x, y, phase = clock() / 8000.0;
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 255));
  SetDCPenColor(hDC, RGB(0, 0, 255));
  for (i = 0; i < N; i++)
  {
    theta = i * pi / N;
    x = xc + r * sin(theta) * cos(phase);
    y = yc - r * cos(theta);
    MoveToEx(hDC, x, y, NULL);
    for (j = 0; j < M; j ++)
    {
      phi  = j * 2 * pi / (M - 1) + phase;
      x = xc + r * sin(theta) * cos(phi);
      y = yc - r * sin(phi);
      //zc = N * cos(theta);
    
      SetDCPenColor(hDC, RGB(255, 0, 255));
      SelectObject(hDC, GetStockObject(DC_PEN));
      LineTo(hDC, x, y);
    }
  }
  for (j  = 0; j < N; j++)
  {
    phi  = j * 2 * pi / (M - 1) + phase;
    x = xc + r * sin(theta) * cos(phi);
    y = yc - r * sin(phi);
    theta = i * pi / N;
    x = xc ;
    y = yc - r ;
    MoveToEx(hDC, x, y, NULL);
    for (j = 0; j < M; j ++)
    {
      theta = i * pi / N;
      phi  = j * 2 * pi / (M - 1) + phase;
      //zc = N * cos(theta);
    
      SetDCPenColor(hDC, RGB(255, 0, 255));
      SelectObject(hDC, GetStockObject(DC_PEN));
      LineTo(hDC, x, y);
    }
}