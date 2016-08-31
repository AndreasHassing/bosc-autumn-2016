#include <stdio.h>

#define IN  1 // inside a word
#define OUT 0 // outside a word

int is_whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\t';
}

int main() {
  int c, state;

  state = OUT;
  while ((c = getchar()) != EOF) {
    // if we are currently outside of a word, and the current
    // character is not whitespace, we are inside a word
    if (state == OUT && !is_whitespace(c)) {
      state = IN;
    }

    // if we are inside of a word,
    if (state == IN) {
      if (is_whitespace(c)) {
        // set the state to out if we hit a whitespace character,
        state = OUT;
        putchar('\n');
      } else {
        // otherwise print the current character, as we are inside
        // a word still
        putchar(c);
      }
    }
  }
}
