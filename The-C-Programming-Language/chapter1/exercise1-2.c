#include <stdio.h>

int main() {
  // The C compiler does not understand \c, thus, a c is inserted
  // in stead.
  printf("hello, \cworld\n");

  return 0;
}
