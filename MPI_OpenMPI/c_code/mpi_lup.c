#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/types.h>
#include <time.h>

#define MAXRAND		(10.0)

void master();
void slave();

void printMatrix( int n, double *a ) 
{
  int	i, j;

  for ( j=0; j<n; j++ ) {
    for ( i=0; i<n; i++ ) {
      printf( "%2.1f ", a[ i+j*n ] );
    }
    printf( "\n" );
  }
}

void initializeMatrix( int n, double *a, double *rhs ) 
{
  int	i, j;

  srand( 1 );
  for ( j=0; j<n; j++ ) {
    for ( i=0; i<n; i++ ) {
      a[ i+j*n ] = (double) rand() / MAXRAND ;
      if ( i==j ) a[ i+j*n ] *= 10 ;
    }
  }

  for ( j=0; j<n; j++ ) rhs[ j ] = 0.0 ;
  for ( j=0; j<n; j++ ) {
    for ( i=0; i<n; i++ ) {
      rhs[ i ] += a[ i+j*n ] ;
    }
  }
}

void checkResult( int n, double *a, double *rhs ) {
  int		i, j, bogus = 0;
  double	*y, diff, max_diff;

  y = (double *) malloc( n*sizeof(double) ) ;

  for ( j=0; j<n; j++ ) y[ j ] = rhs[ j ] ;
  for ( j=0; j<n; j++ ) {
    y[ j ] = y[ j ] / a[ j+j*n ] ;
    for ( i=j+1; i<n; i++ ) {
      y[ i ] -= a[ i+j*n ] * y[ j ] ;
    }
  }
  for ( j=n-1; j>=0; j-- ) {
    for ( i=0; i<j; i++ ) {
      y[ i ] -= a[ i+j*n ] * y[ j ] ;
    }
  }

  max_diff = 0.0;
  for ( j=0; j<n; j++ ) {
    diff = y[ j ] - 1.0 ;
    if ( fabs(diff) > 0.01 ) {
      bogus = 1 ;
      max_diff = diff ;
    }
  }
  if ( bogus ) printf( "TEST FAILED: (%.2f diff)\n", max_diff ) ;
  else printf( "TEST PASSED\n" ) ;

  free( y ) ;
}

void daxpy( double *a, double *b, int n, double alpha ) 
{
  int	i;

  for ( i=0; i<n; i++ ) {
    a[ i ] += alpha * b[ i ];
  }
}

void lu0( double *a, int bsize, int stride ) {
  int		j, k;
  double	alpha;

  for ( k=0; k<bsize; k++ ) {
    /* modify subsequent columns */
    for ( j=k+1; j<bsize; j++ ) {
      a[ k+j*stride ] /= a[ k+k*stride ] ;
      alpha = -a[ k+j*stride ] ;
      daxpy( &a[ k+1+j*stride ], &a[ k+1+k*stride ], bsize-k-1, alpha ) ;
    }
  }
}

void bdiv( double *a, double *diag, int bsize, int stride ) {
  int		j, k;
  double	alpha;

  for ( k=0; k<bsize; k++ ) {
    for ( j=k+1; j<bsize; j++ ) {
      alpha = -diag[ k+j*stride ] ;
      daxpy( &a[ j*stride ], &a[ k*stride ], bsize, alpha ) ;
    }
  }
}

void bmodd( double *a, double *diag, int bsize, int stride ) {
  int		j, k;
  double	alpha;

  for ( k=0; k<bsize; k++ ) {
    for ( j=0; j<bsize; j++ ) {
      a[ k+j*stride ] /= diag[ k+k*stride ] ;
      alpha = -a[ k+j*stride ] ;
      daxpy( &a[ k+1+j*stride ], &diag[ k+1+k*stride ], bsize-k-1, alpha );
    }
  }
}

void bmod( double *a, double *row, double *col, int bsize, int stride ) {
  int		j, k;
  double	alpha;

  for ( k=0; k<bsize; k++ ) {
    for ( j=0; j<bsize; j++ ) {
      alpha = -col[ k+j*stride ] ;
      daxpy( &a[ j*stride ], &row[ k*stride ], bsize, alpha ) ;
    }
  }
}

int myrank;
int matrix_size;//size of the matrix
int block_size; //size of the block

main(int argc, char* argv[]) {

  int myrank;

  /* read in command-line arguments and check validity */
  if ( argc < 3 ) {
    printf( "usage: %s <matrix_size> <block_size>\n", argv[0] ) ;
    exit( -1 );
  } else {
    matrix_size = atoi( argv[1] ) ;
    block_size = atoi( argv[2] ) ;
    if ( (matrix_size == 0) || (block_size == 0) || ((matrix_size-((matrix_size/block_size)*block_size)) != 0) ) {
      printf( "arguments do not satisfy program assumptions:\n"
	      "\t matrix_size > 0, block_size > 0, and\n"
	      "\t block_size perfectly divides matrix_size\n" ) ;
      exit( -1 ) ;
    }
  }

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

  if (myrank==0) {
    master();
  } else {
    slave();
  }

  MPI_Finalize();
}


void master() {
  
  double *a;
  double *rhs;
  int ntasks,pid;
  double *buffer;
  double *workbuf;
  int i,j,k;
  MPI_Status status;
  time_t t0,t1;
  int  ct;
 
  /* get the number of the processes in application. */
  MPI_Comm_size(MPI_COMM_WORLD,&ntasks); 

  /* allocate matrix, rhs vector */
  a = (double *) malloc( matrix_size*matrix_size*sizeof(double) ) ;
  rhs = (double *) malloc( matrix_size*sizeof(double) ) ;

  /* initialize the matrix */
  /* Do we need to allocate a matrix a or ony a row 
     and initilize each row and then send to corresponding process? */
  initializeMatrix( matrix_size, a, rhs );

  /* Send each row to the corresponding process. */
  for (i=block_size;i<matrix_size;i+=block_size) {

    /* send i row to i+block_size-1 row to process 
       (i mod block_size)%ntasks. */
    pid=(i/block_size)%ntasks;

    if (pid!=0)
      MPI_Send(&a[i*matrix_size],matrix_size*block_size,MPI_DOUBLE,
	       pid,i,MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  /* allocate buffer space, it should be big enough 
     to contaion a whole row of block. */
  buffer=(double *)malloc(matrix_size*block_size*sizeof(double));
  
  time(&t0);

  /* Do the computation work of process 0 */
  for (i=0;i<matrix_size;i+=block_size) {

    /* compute the id of the processor that own the row i 
       to row i+block_size-1 */
    pid=(i/block_size)%ntasks;

    if (pid==0) { /* matser process. Me! */
     
      /* factor diagonal */
      lu0(&a[i+i*matrix_size],block_size,matrix_size);

      /* modify "column" by diagonal */
      for (j=i+block_size;j<matrix_size;j+=block_size)
	bdiv(&a[j+i*matrix_size],&a[i+i*matrix_size],
	     block_size,matrix_size);

      /* send this row to other processes, only need to send 
	 the column after diagonal? */
      for (j=1;j<ntasks;j++) {
	MPI_Send(&a[i*matrix_size],block_size*matrix_size,MPI_DOUBLE,
		 j,i,MPI_COMM_WORLD);
      }
      workbuf=&a[i*matrix_size];
    }
    else { /* other process */
      /* receive row i to row i+block_size-1 from process pid */
      MPI_Recv(&buffer,block_size*matrix_size,MPI_DOUBLE,pid,i,
	       MPI_COMM_WORLD,&status);
      workbuf=buffer;
    }

    /* modify the "row" using diagonal */
    for (j=i+(ntasks-pid)*block_size;j<matrix_size;j+=block_size*ntasks) 
      bmodd(&a[i+j*matrix_size],&workbuf[i],
	    block_size,matrix_size);

    /* modify the internal rows and columns */
    for (j=i+(ntasks-pid)*block_size;j<matrix_size;j+=block_size*ntasks)
      for (k=i+block_size;k<matrix_size;k+=block_size) 
	bmod(&a[k+j*matrix_size],&workbuf[k],&a[i+j*matrix_size],
	     block_size,matrix_size);

  }

  MPI_Barrier(MPI_COMM_WORLD);

  time(&t1);
  ct=t1-t0;
  printf("LU decomposition took %d millisecs\n", ct);

  /* Receive the modified matrix from all other processes. */
  for (i=0;i<matrix_size;i+=block_size) {

    /* compute the id of the processor that own the row i 
       to row i+block_size-1 */
    pid=(i/block_size)%ntasks;

    if (pid!=0)
      MPI_Recv(&a[i*matrix_size],block_size*matrix_size,MPI_DOUBLE,pid,i,MPI_COMM_WORLD,&status);
  }
  
  /* test the resulting decoposition */
  checkResult(matrix_size,a,rhs);
  
}

void slave() {

  double *b;
  double *buffer;
  double *workbuf;
  int i,j,k;
  int myrow,nextrow,rownum;
  MPI_Status status;
  int ntasks,pid;

  /* get the number of the processes in application. 
     Can we define ntasks as shared variable?*/
  MPI_Comm_size(MPI_COMM_WORLD,&ntasks); 
  rownum=matrix_size/(block_size*ntasks);
  
  /* allocate the local portion of matrix */
  b=(double*)malloc(rownum*block_size*matrix_size);

  /* allocate buffer space, it should be big enough 
     to contaion a whole row of block. */
  buffer=(double *)malloc(matrix_size*block_size*sizeof(double));

  /* receive the initial matrix from process 0 */
  for (i=0;i<rownum;i++)
    MPI_Recv(&b[i*block_size*matrix_size],block_size*matrix_size,MPI_DOUBLE,
	     0,i*block_size*ntasks+myrank*block_size,MPI_COMM_WORLD,&status);

  MPI_Barrier(MPI_COMM_WORLD);

  /* do computation work of this process */
  for (i=0;i<matrix_size;i+=block_size) {

    /* compute the id of the process that owns the row i 
       to row i+block_size-1 */
    pid=(i/block_size)%ntasks;
    myrow=((i/block_size)/ntasks)*block_size;

    if (pid==myrank) { /* My process */
     
      /* factor diagonal */
      lu0(&b[myrow+myrow*matrix_size],block_size,matrix_size);

      /* modify "column" by diagonal */
      for (j=myrow+block_size;j<matrix_size;j+=block_size)
	bdiv(&b[j+myrow*matrix_size],&b[myrow+myrow*matrix_size],
	     block_size,matrix_size);

      /* send this row to other processes, only need to send the column 
	 after diagonal? */
      for (j=0;j<ntasks;j++) {
	if (j!=myrank)
	  MPI_Send(&b[myrow*matrix_size],block_size*matrix_size,MPI_DOUBLE,
		   j,i,MPI_COMM_WORLD);
      }
      workbuf=&b[myrow*matrix_size];
    }
    else { /* other process */
      /* receive row i to row i+block_size-1 from process pid */
      MPI_Recv(&buffer,block_size*matrix_size,MPI_DOUBLE,pid,i,
	       MPI_COMM_WORLD,&status);
      workbuf=buffer;
    }
    
    if (myrank>pid)
      nextrow=myrow;
    else
      nextrow=myrow+block_size;

    /* modify the "row" using diagonal */
    for (j=nextrow;j<matrix_size;j+=block_size) 
      bmodd(&b[i+j*matrix_size],&workbuf[i],
	    block_size,matrix_size);
     
    /* modify the internal rows and columns */
    for (j=nextrow;j<matrix_size;j+=block_size)
      for (k=i+block_size;k<matrix_size;k+=block_size) 
	bmod(&b[k+j*matrix_size],&workbuf[k],&b[i+j*matrix_size],
	     block_size,matrix_size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  /* Send b to process 0. */
  for (i=0;i<rownum;i++)
    MPI_Send(&b[i*block_size*matrix_size],block_size*matrix_size,MPI_DOUBLE,
	     0,i*block_size*ntasks+myrank*block_size,MPI_COMM_WORLD);
}

