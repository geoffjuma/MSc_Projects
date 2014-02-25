/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>


int main(void)
{
  int a, b;
  int x, y;
  a = 6;
  asm("mrs %[result], apsr" : [result] "=r" (x) : );
  b = a - 182947;
  asm("mrs %[result], apsr" : [result] "=r" (y) : );

  printf("a's negatory is %d\n", b);
  printf("Before negate, apsr was %x\n",x);
  printf("After negate, apsr is %x\n",y);

  return 0;
}

