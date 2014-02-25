#include <stdio.h>
#include <omp.h>
#define NBIN 100000
#define NUM 8
void main() {
double step,sum=0.0,pi;
step = 1.0/NBIN;

# pragma omp parallel
	{
		int nthreads,tid,i;
		double x;
		omp_set_num_threads(NUM);
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num(); 
		printf("I am thread %d of %d forked\n",tid, nthreads);

	for (i=tid; i<NBIN; i+=nthreads) 
	{
	x = (i+0.5)*step;
		#pragma omp critical
		sum += 4.0/(1.0+x*x);
	}
}
pi = sum*step;
printf("PI = %f\n",pi);
}

