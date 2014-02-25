#include <mpi.h>
#include <stdio.h>
#define INTERVAL 100000
int main (int argc, char **argv)
{
        int size,rank;
                double area,localsum, globalsum, mi;
        int i;
        double elapsed_time;
        long start,end;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
                MPI_Barrier(MPI_COMM_WORLD);
        elapsed_time = -MPI_Wtime();
        start = rank * (INTERVAL/size);
        end = (rank+1) * (INTERVAL/size)-1;
        localsum = 0.0;
        for (i = start; i <= end; i++){
        mi = (1.0 / INTERVAL) * (i + 0.5);
        localsum += 4.0 / (1.0 + mi * mi);   mi = (1.0 / INTERVAL) * (i + 0.5);
                localsum += 4.0 / (1.0 + mi * mi);
    }
     MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0) {
                        area = globalsum * (1.0 / INTERVAL);
        printf ("Rank:%d, PI is %.25f \n",rank, area);
        elapsed_time += MPI_Wtime();
        printf ("Total elapsed time: %.6f\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;
}

