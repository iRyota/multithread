#include <stdio.h>
#define N 10000

int main ()
{
  int i, a[N];
  int sum=0;

  for (i=0; i<N; i++) a[i] = i+1;

#pragma omp parallel for
  for (i=0; i<N; i++){
    #pragma omp critical
    sum += a[i];
  }
  printf("sum = %d\n",sum);
  
  return 0;
}
