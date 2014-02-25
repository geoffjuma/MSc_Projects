#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define num = 8
int main (int argc, char *argv[]) {



/* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel
 {
  int nthreads, tid;
 /* Obtain thread number */
 //omp_set_num_threads(8);
 nthreads = omp_get_num_threads();
 tid = omp_get_thread_num();
 printf("Hello World from thread = %d\n", tid);

 /* Only master thread does this */
 if (tid == 0) 
 {
  
 printf("Number of threads = %d\n", nthreads);
 }

 } /* All threads join master thread and disband */

}

