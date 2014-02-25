#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int main(int argc, char *argv[])
{
double start, stop; // for keeping track of running time 
double A[SIZE][SIZE];
double col[SIZE], row[SIZE];
int i, j, k, n;

int num_threads = 4;
if (argc > 1)
{
num_threads = atoi(argv[1]);
}
omp_set_num_threads(num_threads);


// preload A with random values
for (i = 0; i<SIZE; i++)
{
for (j = 0; j<SIZE; j++)
{
A[i][j] = rand();
}
}

// time start now 
start = clock();

// The core algorithm 
for (k = 0; k<SIZE-1; k++)
{
// set col values to column k of A 
for (n = k; n<SIZE; n++)
{
col[n] = A[n][k];
}

// scale values of A by multiplier
for (n = k+1; n<SIZE; n++)
{
A[k][n] /= col[k];
}

// set row values to row k of A 
for (n = k+1; n<SIZE; n++)
{
row[n] = A[k][n];
}

// Here we update A by subtracting the appropriate values from row
// and column. Note that these adjustments to A can be done in
// any order
#pragma omp parallel for
for (i = k+1; i<SIZE; i++)
{
#pragma omp parallel for shared(A, row, col)
for (j = k+1; j<SIZE; j++)
{
A[i][j] = A[i][j] - row[i] * col[j];
}
}
}

// we're done so stop the timer
stop = clock();

printf("Completed decomposition in %.3f seconds using %d thread(s).\n", (stop-start) / CLOCKS_PER_SEC , num_threads);

return 0;
}
