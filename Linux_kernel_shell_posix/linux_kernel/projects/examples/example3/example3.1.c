/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>

int multiplybytwo(int a);

int main(void)
{
  int a, b, d;
  a = -6;
  b = 8;
  d = multiplybytwo(a) * multiplybytwo(b);
  printf("2a * 2b is %d\n", d);

  return 0;
}

int multiplybytwo(int a)
{
  return a*2;
}
