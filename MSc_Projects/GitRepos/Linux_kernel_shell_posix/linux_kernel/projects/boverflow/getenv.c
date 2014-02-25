/* This file is part of the control flow hijack lab.
 * For educational use as part of the Intro to ARM course at http://www.opensecuritytraining.info/IntroARM.html .
 * License: Creative Commons: Attribution, Share-Alike license
 * For details please refer to http://creativecommons.org/licenses/by-sa/3.0/.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if(argc < 2) {
    printf("Usage: %s <environment variable> \n", argv[0]);
    exit(-1);
  }

  char *addr_ptr;

  addr_ptr = getenv(argv[1]);

  if(addr_ptr == NULL) {
    printf("Environmental variable %s does not exist!\n", argv[1]);
    exit(-1);
  }

  printf("%s is stored at address %p\n", argv[1], addr_ptr);
  return 0;
}
