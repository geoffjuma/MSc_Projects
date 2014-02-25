#include <stdio.h>
#include <mpi.h>

typedef struct { 
    double val; 
    int log; 
} SegScanPair; 

/* the user-defined function  */ 
void segScan( SegScanPair *in, SegScanPair *inout, int *len, MPI_Datatype *dptr ) { 
    int i; 
    SegScanPair c; 
for (i = 0; i < *len; ++i) { 
	if (in->log == inout->log) 
		c.val = in->val + inout->val; 
	else 
		c.val = inout->val; 
	c.log = inout->log; 
	*inout = c; 
	in++; inout++; 
    } 
} 

int main(int argc, char *argv[]) {
	int i, base; 
    SegScanPair  a, answer; 
    MPI_Op       myOp; 
    MPI_Datatype type[2] = {MPI_DOUBLE, MPI_INT}; 
    MPI_Aint     disp[2]; 
    int          blocklen[2] = {1, 1}; 
    MPI_Datatype sspair; 
	int rank, size, k;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank <= 3) {
		a.log = 1;
		a.val = 1;
	} else if (rank <= 7 ) {
		a.log = 0;
		a.val = 1;
	} else {
		a.log = 1;
		a.val = 1;
	}
	/* explain to MPI how type SegScanPair is defined */ 
    MPI_Address(&a, disp); 
    MPI_Address(&a.log, disp + 1); 
    base = disp[0]; 
    for (i = 0; i < 2; ++i) disp[i] -= base; 
    MPI_Type_create_struct(2, blocklen, disp, type, &sspair); 
    MPI_Type_commit(&sspair); 
    /* create the segmented-scan user-op   */ 
    MPI_Op_create((MPI_User_function*)segScan, 0, &myOp); 
    MPI_Scan(&a, &answer, 1, sspair, myOp, MPI_COMM_WORLD); 
	int buf;
	for(i = 0; i < size; ++i) {
		if(rank == i) {
			if(rank > 0) {
				MPI_Recv(&buf, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			printf("rank = %d\n", rank);
			printf("\t%d %f\n", answer.log, answer.val);
			if(rank + 1 < size) {
				buf = rank;
				MPI_Send(&buf, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Finalize();
}
