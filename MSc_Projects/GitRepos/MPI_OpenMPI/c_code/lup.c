/* File: lup.c
 * Author: Geoffrey Juma
 * Version: 1.0 (24 March 2013)
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define real float

#define ABS(a) ((a)<0 ? (-(a)) : (a))

/* forward references */
static void *safeMalloc(size_t n);
static real **allocMatrix(size_t height, size_t width);
static void freeMatrix(real **mat);
static real *allocVector(size_t length);
static void freeVector(real *vec);
static void showMatrix (size_t n, real **A);
static void showVector (size_t n, real *vec);
static void decomposeLUP(size_t n, real **A, size_t *P);
static void LUPsolve(size_t n, real **LU, size_t *P, real *x, real *b);
static void solve(size_t n, real **A, real *x, real *b);
/********************/

static double timer(void)
{
  struct timeval tm;
  gettimeofday (&tm, NULL);
  return tm.tv_sec + tm.tv_usec/1000000.0;
}

void *safeMalloc(size_t n)
{
  void *ptr;
  ptr = malloc(n);
  if (ptr == NULL)
  {
    fprintf (stderr, "Error: malloc(%lu) failed\n", n);
    exit(-1);
  }
  return ptr;
}

real **allocMatrix(size_t height, size_t width)
{
  real **matrix;
  size_t row;

  matrix = safeMalloc(height * sizeof(real *));
  matrix[0] = safeMalloc(width*height*sizeof(real));
  for (row=1; row<height; ++row) 
    matrix[row] = matrix[row-1] + width;
  return matrix;
}

void freeMatrix(real **mat)
{
  free(mat[0]);
  free(mat);
}

real *allocVector(size_t length)
{
  return safeMalloc(length*sizeof(real));
}


void freeVector(real *vec)
{
  free(vec);
}


void showMatrix (size_t n, real **A)
{
  size_t i, j;
  for (i=0; i<n; ++i)
  {
    for (j=0; j<n; ++j)
    {
      printf ("%f ", A[i][j]);
    }
    printf ("\n");
  }
}

void showVector (size_t n, real *vec)
{
  size_t i;
  for (i=0; i<n; ++i)
  {
    printf ("%f ", vec[i]);
  }
  printf ("\n");
}

void decomposeLUP(size_t n, real **A, size_t *P)
{
  /* computes L, U, P such that A=L*U*P */
  
  int h, i, j, k, row;
  real pivot, absval, tmp;

  for (i=0; i<n; ++i)
  {
    P[i] = i;
  }

  for (k=0; k<n-1; ++k)
  {
    row = -1;
    pivot = 0;
   
    for (i=k; i<n; ++i)
    {
      absval = (A[i][k] >= 0 ? A[i][k] : -A[i][k]);
      if (absval>pivot)
      {
          pivot = absval;
          row = i;
      }
    }
    
    if (row == -1)
    {
      printf ("Singular matrix\n");
      exit(-1);
    }
    
    /* swap(P[k],P[row]) */
    h = P[k];
    P[k] = P[row];
    P[row] = h;

    /* swap rows */
    /*#pragma omp parallel shared(A) private(i, j, tmp) firstprivate(k)*/
    {
      /*#pragma omp for*/
      for (i=0; i<n; ++i)
      {
        tmp = A[k][i];
        A[k][i] = A[row][i];
        A[row][i] = tmp;
      }

      /*#pragma omp barrier*/

      /*#pragma omp for*/
      for (i=k+1; i<n; ++i)
      {
        A[i][k] /= A[k][k];
        for (j=k+1; j<n; ++j)
        {
          A[i][j] -= A[i][k]*A[k][j];
        }
      }
    }
  }
}

void LUPsolve(size_t n, real **LU, size_t *P, real *x, real *b)
{
  real *y;
  size_t i, j;

  /* Solve Ly=Pb using forward substitution */
  y = x;  /* warning, y is an alias for x! It is safe, though. */
  /* Writes y[i], reads y[0..i-1], so sequential dependency, ouch. */
  for (i=0; i<n; ++i)
  {
    y[i] = b[P[i]];
    for (j=0; j<i; ++j)
    {
      y[i] -= LU[i][j]*y[j];
    }
  }
  
  /* Solve Ux=y using backward substitution */
  /* This is really for(i = n-1; i >= 0; i--) */
  /* Writes x[i], reads x[i+1..n-1], so sequential dependency. */
  i=n;
  while (i>0)
  {
    i--;
    x[i] = y[i];
    for (j=i+1; j<n; ++j)
    {
      x[i] -= LU[i][j]*x[j];
    }
    x[i] /= LU[i][i];
  }
}

void solve(size_t n, real **A, real *x, real *b)
{
  size_t *P;

  /* Construct LUP decomposition */
  P = safeMalloc(n*sizeof(size_t));
  decomposeLUP(n, A, P);
  /* Solve by forward and backward substitution */
  LUPsolve(n, A, P, x, b);

  free(P);
}


/**
 * Inverts a square matrix A (n by n) and puts the result in Ainv
 */
void invert(size_t n, real **A, real **Ainv)
{
  real *b, tmp, **Acopy;
  size_t i, j, k;
  #pragma omp parallel private(b, j, tmp, Acopy) shared(A, Ainv) firstprivate(n)
  {
    #pragma omp for
    for(i = 0; i < n; i++)
    {
      /* Make a copy of A because solve() modifies it; ARGH */
      Acopy = allocMatrix(n, n);
      for(j = 0; j < n; j++)
        for(k = 0; k < n; k++)
          Acopy[j][k] = A[j][k];

      /* Construct b: the i-th row of the identity matrix */
      b = allocVector(n);
      for(j = 0; j < n; j++)
          b[j] = 0;
      b[i] = 1;
      
      /* Calculate x such that Ax=b , and put x in Ainv[i] (the i-th row of Ainv).
       * It should really be the i-th column, so we'll transpose Ainv at the end. */
      solve(n, Acopy, &Ainv[i][0], b);
      freeMatrix(Acopy);
      freeVector(b);
    }

    #pragma omp barrier

    /* Now transpose Ainv back. */
    #pragma omp for private(tmp)
    for(i = 0; i < n; i++)
      for(j = 0; j < i; j++)
      {
        tmp = Ainv[i][j];
        Ainv[i][j] = Ainv[j][i];
        Ainv[j][i] = tmp;
      }
  }
}

/**
 * Multiplies two square, n-by-n matrices A and B, and puts the result (A*B) in C
 */
void multiply(size_t n, real **A, real **B, real **C)
{
  size_t i, j, k;
  #pragma omp parallel for private(j, k) firstprivate(n) shared(A, B, C)
  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
    {
      C[i][j] = 0;
      for(k = 0; k < n; k++)
        C[i][j] += A[i][k] * B[k][j];
    }
}

void solveMatrixVector(int s, real **A, real *x, real *b)
{
  double t;
  real c, d;
  int i, correct=1;

  showMatrix (s, A);
  printf ("\n");
  
  t = timer();
  solve(s, A, x, b);
  t = timer() - t;
  printf("Solved in %lf seconds.\n", t);
  /*d = -0.5*s;*/
  d = x[0];
  c = -0.5*s +0.5;
  printf("c=%lf, d=%lf", c, d);
  for(i = 0; i < s && correct; i++)
  {
    if(ABS(x[i] - (0.5*(real)i*(real)i+c*(real)i+d)) > ABS(0.10*x[i]))
      correct = 0;
  }
  printf("Solution is %scorrect.\n", correct ? "" : "NOT ");
  
  showVector(s, x);
}

void invertAndCheck(int s, real **A, real **Ainv, real **C)
{
  int i, j, correct=1;
  double t;

  printf("\n");
  t = timer();
  invert(s, A, Ainv);
  t = timer() - t;

  printf("Inverted matrix in %lf seconds.\n", t);
  showMatrix(s, Ainv);

  t = timer();
  multiply(s, A, Ainv, C);
  t = timer() -t;

  printf("Multiplied matrices in %lf seconds.\n", t);

  for(i = 0; i < s; i++)
    for(j = 0; j < s; j++)
      if(ABS(C[i][j] - (i == j ? 1.0 : 0.0)) > 0.01)
        correct = 0;
  printf("Solution is %scorrect.\n", correct ? "" : " NOT");
  if(!correct)
   showMatrix(s, C);
} 

int main(int argc, char **argv)
{
  real **A, *x, *b, **Ainv, **C;
  int s, i, j;

  printf("Matrix size: ");
  scanf("%d", &s);

  A = allocMatrix(s, s);
  Ainv = allocMatrix(s, s);
  x = allocVector(s);
  b = allocVector(s);
  C = allocMatrix(s, s);

  for(i = 0; i < s; i++)
  {
    for(j = 0; j < s; j++)
      A[i][j] = (i == j ? -2 : (j == i - 1 || j == i + 1 ? 1 : 0 ) );
    b[i] = 1;
  }

  /*solveMatrixVector(s, A, x, b);*/
  invertAndCheck(s, A, Ainv, C);
 
  freeMatrix(A);
  freeMatrix(Ainv);
  freeMatrix(C);
  freeVector(x);
  freeVector(b);
  
  return EXIT_SUCCESS;
}

