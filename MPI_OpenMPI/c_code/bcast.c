#include<stdio.h>
#include<mpi.h>
int main(int argc , char **argv)
{
        int size, rank,i;
        int buffer[10];
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        if(rank!=0){
                for(i=0;i<10;i++)
                buffer[i]=0;
        }
        if(rank==0){
                for(i=0;i<10;i++)
                buffer[i]=i*2;
        }
        MPI_Bcast(buffer,10,MPI_INT,0,MPI_COMM_WORLD);
        if(rank!=0){

                for(i=0;i<10;i++)
                printf("Process %d: buffer[%d]=%d\n",rank,i,buffer[i]);
        }
        MPI_Finalize();
}
