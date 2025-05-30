#include <stdio.h>

int main() {
  unsigned int a=1;
  unsigned int b=1;
  unsigned int c=0;
  unsigned int i=0;

  for(i=0; i<10; ++i) {
    c = b;	/* store temporarily */
    b += a;
    a = c;
    printf("%u  %u\n", i, b);
  }
  printf("Done!\n");

  return 0;
}