/* sin.c - parallel C code to demonstrate Linux thread interface 
 * Calcuates the integral of sin from 0 to pi using multiple threads
 * Implemented by Geoffrey Juma
 * Usage: ./a.out <numIntervals> <numThreads>
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

/* global variables (shared by all threads */
volatile long double area = 0.0; /* the approximation, to 31 sigfigs */
pthread_mutex_t piLock;        /* how we synchronize writes to 'pi' */
long double intervals;         /* how finely we chop the integration */
int numThreads;                /* how many threads we use */

/* the function a thread executes
 * Parameters: arg, a void* storing the address of the thread ID.
 */
void *computeArea(void *id)
{
    long double x,
                width,
                localSum = 0;
    int i,
        threadID = *((int*)id);

    width = 360.0/ intervals;

    for(i = threadID ; i < intervals; i += numThreads) {
        x = (i * width);
        localSum += sin(x);
    }

    localSum *= width; 

    pthread_mutex_lock(&piLock);
    area += localSum;
    pthread_mutex_unlock(&piLock);
    printf ("I am thread number :%d My area is %Lf \n",threadID ,localSum); 

    return NULL;
} 

int main(int argc, char **argv)
{
    pthread_t *threads;        /* dynarray of threads */
    void *retval;              /* unused; required for join() */
    int *threadID;             /* dynarray of thread id #s */
    int i;                     /* loop control variable */

   if (argc == 3) {
      intervals = atoi(argv[1]); 
      numThreads = atoi(argv[2]); 
      threads = malloc(numThreads*sizeof(pthread_t));
      threadID = malloc(numThreads*sizeof(int));
      pthread_mutex_init(&piLock, NULL);

      for (i = 0; i < numThreads; i++) {
        threadID[i] = i;
        pthread_create(&threads[i], NULL, computeArea, threadID+i);
      }

      for (i = 0; i < numThreads; i++) {
         pthread_join(threads[i], &retval);
      }
      printf("Estimation of the area under the curve  is %Lf \n", area);
    } else {
      printf("Usage: ./a.out <numIntervals> <numThreads> \n");    
    }

    return 0;
}

