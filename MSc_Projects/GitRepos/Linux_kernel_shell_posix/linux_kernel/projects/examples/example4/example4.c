/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>

int multiply(int a, int b);

int main(void)
{
  int a, b, d;
  a = 221412523;
  b = 3;
  d = multiply(a,b);
  printf("a * b is %d\n", d);

  return 0;
}

int multiply(int a, int b)
{
  return (a*b);
}
