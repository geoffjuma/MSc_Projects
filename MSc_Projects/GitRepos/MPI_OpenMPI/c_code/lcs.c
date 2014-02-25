# include <math.h>
# include <stdio.h>
void main (int argc, char* argv[])
{
 int i,j;
 FILE *fp1,*fp2;
 fp1 = fopen("/home/ashley/A.txt","rb+");
 fp2 = fopen("/home/ashley/B.txt", "rb+");
 int M = strlen (&fp1);
 int N = strlen (&fp2);
 int LCS[M][N];
 int X[i],Y[j];
  for (i=0; i<M; i++)
     {
      LCS[i][0]=0;
     }
  for (j=0; j<N; j++)
     {
      LCS[0][j]=0;
     }
  for (i=0; i<=M; i++)
     {
     for (j=0; j<=N; j++)
        {
         if  (X[i]==Y[j])
           LCS[i][j] =1+LCS[i-1][j-1];
         else
           LCS[i][j] = max(LCS[i-1][j],LCS[i][j-1]);
          printf("%d\n",LCS[i][j]);
        }
    }
  
}

 int max (int a, int b)
{
 if (a>b)
 return a;
 else return b;
}
