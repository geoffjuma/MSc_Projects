
#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>

//define the matrices
#define row_A 3
#define column_A 3 
#define row_B 4
#define column_B 3
#define row_C 3
#define column_C 3
//global matrix
int A[row_A][column_A],B[row_B][column_B],C[row_C][column_C];
//the main program starts here people
void main (int argc, char* argv[])
{
		
	int row,extra,offset,upperrow;
	int tag=0;
	int size,myrank;
	MPI_Status status;
	double start,stop;
//initialize MPI

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

printf("we are %d bitches here\n",myrank);
if (myrank==0)
{
printf("----Matrix A-----\n");
int i,j;

     for (i=0;i<=row_A;i++)
       {
	for (j=0; j<=column_A;j++)
	  {
	 A[i][j]= i*j;
	 printf("%d ",A[i][j]);
	 }
    printf("\n");
   }
printf("----Matrix B-----\n");
     for (i=0;i<=row_A;i++)
       {
	for (j=0; j<=column_A;j++)
	  {
	 A[i][j]= i+j;
	 printf("%d ",A[i][j]);
	 }
    printf("\n");
   }
//start distributing them
upperrow=row_A/size-1;
extra=row_A%size-1;
offset=0;

//we send row by row increasing

for(i=1;i<=size;i++)
	{
	if (dest <= extra)
        row = upperrow+1;
        else
         row = upperrow;

    MPI_Send(&row, 1, MPI_INT,i,tag, MPI_COMM_WORLD);
    count = row*column_A;
    MPI_Send(&a[offset][0], count, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
    count = column_A*column_B;
    MPI_Send(&B, count, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
    offset = offset + row;
	}
}
MPI_Finalize();

}

