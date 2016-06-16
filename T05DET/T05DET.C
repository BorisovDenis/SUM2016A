/*
Programm: T05DET
Programmer: Borisov Denis
Date: 16.06.2016
*/
#include <stdio.h>
#define MAX 3
int N;
int P[MAX], Parity = 0;
double A[MAX][MAX], sum = 0;

/*start of "LoadMatrix" function*/
void LoadMatrix( char *FileName )
{
  int i, j;

  FILE *F;
  F = fopen("file.log", "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &N);
  for (i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
} /*end of "LoadMatrix" functoin*/
/*start of "Swap" function*/
void Swap( int *A, int *B )
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}/* End of "Swap" function */
/*start of "Go" function*/
void Go( int Pos )
{
  int i;
  double prod = 1;

 if (Pos == N)
  {
    for(i = 0; i < N; i++) 
      prod *= A[i][P[i]];
    if (Parity == 0)
      sum += prod;
    else
      sum -= prod;
    return;
  }
 else
 {
   Go(Pos + 1);
   for (i = Pos + 1; i < N; i++)
   {
     Swap(&P[Pos], &P[i]);
     Parity = !Parity;
     Go(Pos + 1);
     Swap(&P[Pos], &P[i]);
     Parity = !Parity;
   }
 }
}/* End of "Go" function */
/* Start of "EvalDeterminant" function */
double EvalDeterminant( char *FileName )
{
  int i;

  LoadMatrix("file.log");
  sum = 0;
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  return sum;
}/* End of "EvalDeterminant" function */
/* Start of "EvalDeterminant" function */
void main( void )
{
  int i;
  char *M[] =
  {
    "mat2.txt"
  };

  for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
    printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
} /* End of main function */