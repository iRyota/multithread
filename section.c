#include <stdio.h>
#include <omp.h>
#define N 5

int main ()
{
  int i, j, a[N], b[N], c[N], d[N];
  for (i = 0; i < N; i++) a[i] = b[i] = i;

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
	for (i = 0; i < N; i++) {
	  c[i] = a[i] + b[i];
	  printf("Thread %d: c[%d] = %d\n", omp_get_thread_num(), i, c[i]);
	}
      }
      #pragma omp section
      {
	for (j = 0; j < N; j++) {
	  d[j] = a[j] * b[j];
	  printf("Thread %d: d[%d] = %d\n", omp_get_thread_num(), j, d[j]);
	}
      }
    }
  }

  return 0;
}
