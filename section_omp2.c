#include <omp.h>
#include <stdio.h>
#define N 5

int main ()
{
  int i,a[N], b[N], c[N], d[N];
  for (i=0; i<N; i++) a[i] = b[i] = i;
#pragma omp parallel private(i)
  {
#pragma omp sections
    {
#pragma omp section
      {
	for (i=0; i<N; i++){
	  c[i] = a[i] + b[i];
	  printf("Thread %d: c[%d]= %d\n",omp_get_thread_num(),i,c[i]);
	}
      }
#pragma omp section
      {
	for (i=0; i<N; i++){
	    d[i] = a[i] * b[i];
	    printf("Thread %d: d[%d]= %d\n",omp_get_thread_num(),i,d[i]);
	}
      }
    }
  }
  return 0;
}
