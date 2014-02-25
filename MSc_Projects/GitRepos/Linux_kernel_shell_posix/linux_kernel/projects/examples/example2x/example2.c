/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>

extern int multiply(int a, int b);
extern int multiplyadd(int a, int b, int c);

int main(void)
{
  int a, b, c, d;
  a = 0x0A;
  b = 0x0A0B0C0D;
  c = 4;
  d = multiply(a,b);
  printf("a * b is %d\n", d);
  d = multiplyadd(a,b,c);
  printf("a * b + c is %d\n", d);

  return 0;
}

