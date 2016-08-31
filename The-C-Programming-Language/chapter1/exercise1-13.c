#include <stdio.h>

#define IN  1
#define OUT 0
#define WORD_MAXLENGTH 30

int main() {
  int c, state, wl;
  int nwords_of_length[WORD_MAXLENGTH];
  for (int i = 0; i < WORD_MAXLENGTH; i++)
    nwords_of_length[i] = 0;

  state = OUT;
  wl = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') {
      ++nwords_of_length[wl];
      wl = 0;
    } else {
      ++wl;
    }
  }

  for (int i = 1; i < WORD_MAXLENGTH; i++) {
    if (nwords_of_length[i] > 0) {
      printf("%2d: ", i);
      for (int j = 0; j < nwords_of_length[i]; j++)
        printf("#");
      printf("\n");
    }
  }
}
