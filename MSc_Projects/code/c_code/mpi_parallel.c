# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>

void main (int argc, char* argv[])
{
	MPI_Status status;
	int size, myrank;
	char a[]="APBCADCQER"; //the strings to be compared
	char b[]="RASBTAUCVE";
	int n = strlen(a); //length of the strings
	int m = strlen(b); 
	
//initialize MPI
	MPI_Init(&argc,&agrv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	for (int j=0; j<=n;i++)
	{	
	  a[j]=0;
	}
	if (myrank==0)
	{
 	  for (int i=0; i<=m;i++)
	  {	
	  MPI_Bcast(&a,1,MPI_INT,0,MPI_COMM_WORLD);
	   for (int j=0;j<=n;j++)
	      {
		a[i]=b[j];
		}
	    }
         for ( j=0; j<=n;j++)
           {
             b[j]=0;
              }
          for(j=0; j<n; j++)
             {
             b[j+1]=a[j]; 
             }
}
