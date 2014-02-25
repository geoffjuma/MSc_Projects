#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
 int myrank,size,length=100;
 char myname[MPI_MAX_PROCESSOR_NAME];
//initialize MPI 
 MPI_Init(&argc,&argv);
 //get the MPI parameters
 MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Get_processor_name(myname,&length);
//print out the parameters
printf("Processor Name--> %s Rank --> %d Size --> %d \n",myname,myrank,size);
MPI_Finalize();
}
