
/*
This is an implementation of the algorithm for finding the Longest Common Sequence of two absolute Strings S1,S2
*/

#include<stdio.h> 
#include<string.h> 
# include <time.h>

int max(int a,int b) 
{ 
return a>b?a:b; 
}
//end max() 

int main() 
{ 
 time_t start, stop;
 float diff,t1,t2;
 
char a[]="APBCADCQER"; 
char b[]="RASBTAUCVE";
int n = strlen(a); 
int m = strlen(b); 

//the algorith with dynamic programming
//start time here

time(&start);
int i,j; 
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
                 printf("%d |",l[i][j]); 
                 } 
                printf("\n\t"); 
               } 
printf("Length of Longest Common Subsequence = %d\n",l[n][m]); 
return 0; 
time(&stop);
diff = difftime(start, stop);
printf("the time taken is %f\n",diff );
}


