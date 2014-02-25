/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>
int saturate(int val, int amin, int amax);
int main(void)
{
  int a, b;
  int *x;
  a = 8;
  b = 9;

  x = &a;
  b = *x + 2;

  printf("The value of b is %d\n",b);

  a = saturate(b,10,10);

  printf("The value of b is now %d\n",a);
  
  return 0;
}

int saturate(int val, int amin, int amax)
{
  if (val < amin)
    return amin;
  else if(val > amax)
    return amax;
  else
    return val;
}
