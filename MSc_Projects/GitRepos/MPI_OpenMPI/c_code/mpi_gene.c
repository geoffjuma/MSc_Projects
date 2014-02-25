
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//global variables
	char a[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	char b[]="RASBTAUCVE";
	int m = strlen(a); 
	int n = strlen(b);
	int** l;

//the LCS function

int computeLCS(char* s1,char* s2, int len1, int len2)
{
int i,j;	
//compute the LCS algorithm
   for(i=n;i>=1;i--) 
      a[i] = a[i-1]; 
        for(i=m;i>=1;i--) 
           b[i] = b[i-1]; 
          int l[n+1][m+1]; 
          printf("\n\t"); 
             for(i=0;i<=n;i++) 
              { 
              for(j=0;j<=m;j++) 
               { 
                if(i==0 || j==0) 
                  l[i][j]=0; 
                 else if(a[i] == b[j] ) 
                 l[i][j] = l[i-1][j-1] + 1; 
                 else 

                 l[i][j] = max(l[i][j-1],l[i-1][j]); 
                 
                } 
                
               } 

return l[n][m]; 
}

int main (int argc, char* argv[])
{
	
	int block,*chunk,*recvbuffer,tag=0;
	int myrank,name_len,size;
	char proc_name [MPI_MAX_PROCESSOR_NAME];
	MPI_Status status;

//initalize MPI
MPI_Init(&argc,&argv);

//Get the processors parameters
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
MPI_Get_processor_name(proc_name, &name_len);

/*output the processors details
if (size % 2=0)
	{
	printf("we have a small issue here sir! The number of processors must be odd!\n");
	}*/
printf("Hello world, I am %s-->of rank %d\n",proc_name,myrank);

int i,j;
MPI_Barrier(MPI_COMM_WORLD);//synchronize the processors
if (myrank==0)
	{
//Send or Broadcast all elements of array X to all slave processors 
	MPI_Bcast(a,n,MPI_CHAR,0,MPI_COMM_WORLD);
	block = m/size-1;
//scatter the blocks to corresponding slave processors
	MPI_Scatter(b,block,MPI_CHAR,chunk,block,MPI_CHAR,0,MPI_COMM_WORLD);
	}
else
	{
	//recieve the broadcasts
	//call local LCS function
	int computeLCS(char* a,char* block, int len1, int len2);
	//return the results
	}

if (myrank==0)
{
//recieve the results
MPI_Reduce(&chunk,recvbuffer,10,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
printf("Length of Longest Common Subsequence = %d\n",l[n][m]); 
//Print out the resulting matrix
//end the computation
}
MPI_Finalize();
}		

