#include <stdio.h>

void reverse(char s[], int len) {
  char temp;
  char reversed[len];

  for (int i = 0; i < len; i++) {
    temp = s[i];
    s[i] = s[len-i-1];
    s[len-i-1] = temp;
  }
}

int main() {
  char c;
  char line[500];
  for (int i = 0; i < 500; i++) {
    line[i] = '\0';
  }

  int i = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      if (i == 0) continue;
      reverse(line, i);
      printf("%s\n", line);
      i = 0;
      continue;
    }

    line[i++] = c;
  }
}
