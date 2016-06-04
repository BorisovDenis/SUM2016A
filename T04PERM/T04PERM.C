// Borisov Denis
#include <stdio.h>
#define n 5
static int p[n] = {1, 2, 3, 4, 5};
int Parity = 0;
void Swap(int *a, int *b)
{
  int i = *a;
  *a = *b;
  *b = i;
}
void SavePerm( void )
{
  int i;
  FILE *F;
  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0 ; i < n - 1; i++)
    fprintf(F, "%2i", p[i]);
  if (Parity)
    fprintf(F, "-Odd");
  else
    fprintf(F, "-Even");
  fprintf(F, "%2i \n", p[n - 1]);
  fclose(F);                
}
void Go( int Pos )
{
  int i;                    
  if (Pos == n)
    SavePerm();
  else
   for (i = Pos; i < n ; i++)
   {
     if (Pos != i)
       Swap(&p[Pos], &p[i]), Parity = !Parity ;
     Go(Pos + 1);
     if (Pos != i)
       Swap(&p[Pos], &p[i]), Parity = !Parity ;
   }
}

void main( void )
{
 int i;
 for (i = 0; i < n ; i++)
   p[i] = i + 1;
 Go(0);
}