/* FILE: TO5DET.C
*Borisov Denis
*DATA: 16.06.2016
*/
#include <stdio.h>
#include <conio.h>
#define MAX 3

int n, P[MAX];
double A[MAX][MAX], sum;
char Parity = 1, SaveParity;

void LoadMatrix( char *FileName )
{
  int i, j;

  FILE *F;
  F = fopen(FileName, "r");
  if (F == NULL)
  return;
  fscanf(F, "%d", &n);
  for (i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
}/*End of "LoadMatrix" function */
void Swap( int *A, int *B )
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}/* End of "Swap" function */

void Go( int Pos )
{
  int i;
  double prod = 1;

  if (Pos == n)
  {
    for(i = 0; i < n; i++) 
      prod *= A[i][P[i]];
    if (Parity)
      sum += prod;
    else
      sum -= prod;
    return;
  }
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < n; i++)
    {
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
         Parity = !Parity;
      Go(Pos + 1);
      if ( Pos != i)
        Parity = !Parity;
      Swap(&P[Pos], &P[i]); 
    }
  }
}

double EvalDeterminant( char *FileName )
{
  int i;

  LoadMatrix(FileName);
  sum = 0;
  for (i = 0; i < n; i++)
    P[i] = i;
  Go(0);
  return sum;
}

void main( void )
{
  int i;
  char *M[] =
  {
  "mat3.txt", "mat1.txt", "mat2.txt", "mat4.txt"
  };
  for (i = 0; i < n; i++)
    P[i] = i;
  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
  _getch(); 
}
/* End of main function */