#include <omp.h>
#include <stdio.h>
#define N 10

int main ()
{
  int i, a[N], b[N], c[N];

  for (i = 0; i < N; i++) {
    a[i] = b[i] = i;
    c[i] = 0;
  }

  #pragma omp parallel for
  for (i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
    printf("Thread %d: c[%d] = %d\n", omp_get_thread_num(), i, c[i]);
  }
  return 0;
}
