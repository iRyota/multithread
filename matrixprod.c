#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

struct matrix { // size次の正方行列
  int *val; // 1次元配列, ij成分: val[i+size*j]
  int size;
};
typedef struct matrix matrix;

matrix *matrix_mul(matrix, matrix);
void print_m(matrix);

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main(int argc, char **argv)
{
  matrix a, b, *c;
  int i, j, n;
  double ti, tf;

  n = atoi(argv[1]);

  a.val = (int *)malloc(sizeof(int)*n*n);
  b.val = (int *)malloc(sizeof(int)*n*n);
  
  a.size = b.size = n;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      a.val[i+n*j] = (rand() % 11) - 5;
      b.val[i+n*j] = (rand() % 11) - 5;
    }
  }

  printf("starting calculation\n");
  ti = gettimeofday_sec();
  
  c = matrix_mul(a, b);

  tf = gettimeofday_sec();
  printf("matrix a:\n");
  print_m(a);
  printf("matrix b:\n");
  print_m(b);
  printf("matrix a×b:\n");
  print_m(*c);

  printf("time = %lf [sec]\n", (double)(tf - ti));

  return 0;
}

/**
 * 行列の積を計算する
 */
matrix *matrix_mul(matrix a, matrix b) {
  int i, j, k, n;
  matrix *c;

  n = a.size;
  c = (matrix *)malloc(sizeof(matrix));
  c->val = (int *)malloc(sizeof(int)*n*n);
  c->size = n;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      c->val[i+n*j] = 0;
      for (k = 0; k < n; k++) {
	c->val[i+n*j] += a.val[k+n*i]*b.val[j+n*k];
      }
    }
  }

  return c;
}

/**
 * 行列を表示する
 */
void print_m(matrix A)
{
  int i, j, n;

  n = A.size;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      printf("%d\t",A.val[i+n*j]);
    }
    printf("\n");
  }
  
  return;
}
