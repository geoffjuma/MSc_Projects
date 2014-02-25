/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>

extern int divide(int a, int b);
extern int unsigneddivide(unsigned int a, unsigned int b);

int main(void)
{
  int a, b, d;
  a = -6;
  b = 2;
  d = divide(a,b);
  printf("a / b is %d\n", d);
  d = unsigneddivide(b,b);
  printf("b / b unsigned is %d\n", d);

  return 0;
}

