# include <stdio.h>
# include <stdlib.h>

void main (int argc, char* argv[])
{
 int one,two,myarea1,myarea2;
 printf("please enter the two numbers \n");
 scanf("%d%d",&one,&two);
 myarea1 = findArea(&one,&two);
 myarea2 =findArea1(one,two);
 printf("the area is : %\t\n",myarea1);
 printf("the perimter is : %d \n",myarea2);
 //printf ("the number of arguments %d\t%s\n",argc,argv[0]);
 
}
int findArea(int* x, int* y)
{
 int area =(x * y);
 return area;
}

int findArea1(int x, int y)
{
 int area= (x*y);
 return area;
}
