/* This file is part of the examples given in the slide.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 */
#include <stdio.h>

int checksum(int *data);

int main(void)
{
  int result;
  int x[8] = {2, 5, 6, 9, 17, 28, 13, 27};
  result = checksum(x);

  printf("The checksum of x is now %d\n",result);
  return 0;
}

int checksum(int *data)
{
  unsigned i;
  int sum = 0;

  for(i=7; i>=0; i--)
    sum += *data++;

  return sum;
}
