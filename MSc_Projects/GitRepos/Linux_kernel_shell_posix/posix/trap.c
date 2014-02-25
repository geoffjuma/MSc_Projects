#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "time.h"

/* The global variables are shared among all the threads. */
/* All of them except total and mutex are set in main.    */
int     thread_count;
double  a, b, h;
int     n, local_n;

/* A mutex is a type of lock.  It can be used to provide     */
/* mutual exclusion for a critical region, in our case, when */
/* a thread's calculation is added into total.               */
pthread_mutex_t   mutex, bar_mutex, max_mutex;
pthread_cond_t    bar_cond;
int bar_count = 0;
double  total;
//timing parameters
//clock_t begin,end;
double  elapsed;


void *Thread_work(void* rank);
double Trap(double local_a, double local_b, int local_n,
           double h);    /* Calculate local integral  */
double f(double x); /* function we're integrating */
void My_barrier(void);


/*--------------------------------------------------------------*/
int main(int argc, char** argv) {
    long        i;
    pthread_t*  thread_handles;  

    

    total = 0.0;
    if (argc != 2) {
       fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
       exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    h = (b-a)/n;
    local_n = n/thread_count;

    /* Allocate storage for thread handles.  Thread_count better */
    /* be positive . . .                                         */
    thread_handles = malloc (thread_count*sizeof(pthread_t));

    /* Initialize the mutex */
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&bar_mutex, NULL);
    pthread_mutex_init(&max_mutex, NULL);
    pthread_cond_init(&bar_cond, NULL);

    /* Start the threads.  Next to last argument is function in */
    /* which thread runs.  Final argument is thread's rank      */
    for (i = 0; i < thread_count; i++) {
         pthread_create(&thread_handles[i], NULL, Thread_work, 
               (void*) i);
    }

    /* Wait for threads to complete. */
    for (i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    printf("With n = %d trapezoids, our estimate\n",
         n);
    printf("of the integral from %f to %f = %19.15e\n",
         a, b, total);
    printf("Elapsed time = %e seconds\n", elapsed);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&bar_mutex);
    pthread_mutex_destroy(&max_mutex);
    pthread_cond_destroy(&bar_cond);
    free(thread_handles);

    return 0;
} /*  main  */

/*--------------------------------------------------------------*/
void *Thread_work(void* rank) {
    double      local_a;   /* Left endpoint my thread   */
    double      local_b;   /* Right endpoint my thread  */
    double      my_int;    /* Integral over my interval */
    long        my_rank = (long) rank;
    clock_t      begin, end;
    double  my_elapsed;

    My_barrier();
    begin = clock();

    /* Length of each process' interval of integration = */
    /* local_n*h.  So my interval starts at:             */
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = Trap(local_a, local_b, local_n, h);

    /* Block thread until mutex is available. */
    pthread_mutex_lock(&mutex);
    total += my_int;
    /* Make mutex available to another thread */
    pthread_mutex_unlock(&mutex);
    end = clock();
    my_elapsed = (double)(begin-end)/CLOCKS_PER_SEC;

    printf("Thread %ld > my_elapsed = %e\n", my_rank, my_elapsed);
    pthread_mutex_lock(&max_mutex);
    if (my_elapsed > elapsed) elapsed = my_elapsed;
    pthread_mutex_unlock(&max_mutex);
    
    return NULL;

}  /* Thread_work */

/*--------------------------------------------------------------*/
double Trap(
          double  local_a   /* in */,
          double  local_b   /* in */,
          int     local_n   /* in */,
          double  h         /* in */) {

    double integral;   /* Store result in integral  */
    double x;
    int i;

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        integral += f(x);
    }
    integral = integral*h;
    return integral;
} /*  Trap  */


/*--------------------------------------------------------------*/
double f(double x) {
    double return_val;

    return_val = sin(x);
    return return_val;
} /* f */

/*--------------------------------------------------------------*/
void My_barrier(void) {
   pthread_mutex_lock(&bar_mutex);
   bar_count++;
   if (bar_count == thread_count) {
      pthread_cond_broadcast(&bar_cond);
   } else {
      while (pthread_cond_wait(&bar_cond, &bar_mutex) != 0);
   }
   pthread_mutex_unlock(&bar_mutex);
}  /* My_barrier */
