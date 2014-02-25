/* This file is part of the fibonacci lab.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 * License: Creative Commons: Attribution, Share-Alike license
 * For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.
 */
#include <stdio.h>

extern int fibonacci(int x);

int main(int argc, char **argv)
{
  int number=0;
  int result=0;

  printf("Please input a number:");
  scanf("%d",&number);
  result = fibonacci(number);   
  printf("The fibonacci sequence at %d is: %d\n", number, result);
}

