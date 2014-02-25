/* This file is part of the control flow hijack lab.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 * License: Creative Commons: Attribution, Share-Alike license
 * For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.
 */
#include <stdio.h>
#include <stdlib.h>

void stage1(char *buffer);

int main(int argc, char **argv)
{
  stage1(argv[1]);

  return 0;
}


void stage1(char *buffer)
{
  char buf[5];
  // Intentional buffer overflow vulnerability
  strcpy(buf, buffer);
  // Intentional format string vulnerability
  printf("%s", buf);

}

