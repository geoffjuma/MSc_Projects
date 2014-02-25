#include<stdio.h>
#include <stdlib.h>
#define n = 4

void main (int argc, char **agv[])
{
int col, row;
int matrix[4][4];
for (row=0;row<=4;row++)
	{
	for (col =0;col<=4;col++)
	{
	printf("\n %d",matrix[row][col]);
	}
	}	
printf("\n");
} 
