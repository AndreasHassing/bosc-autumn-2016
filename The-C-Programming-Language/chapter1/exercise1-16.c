#include <stdio.h>

#define MAXLINE 1000 // maximum input line length

int mygetline(char line[], int maxline);
void copy(char to[], char from[]);

// print the longest input line
int main() {
  int len;               // current line length
  int max;               // maximum length seen so far
  char line[MAXLINE];    // current input line
  char longest[MAXLINE]; // longest line saved here

  max = 0;
  while ((len = mygetline(line, MAXLINE)) > 0) {
    if (len > max) {
      max = len;
      copy(longest, line);
    }
  }

  if (max > 0) {
    // there was a line
    printf("%s", longest);
  }

  return 0;
}

// mygetline: read a line into s, return length
int mygetline(char s[], int lim) {
  int c, i;
  int linelength = 0;

  for (i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
    if (i < lim-1)
      s[i] = c;

    linelength++;
  }

  if (c == '\n') {
    s[i] = c;
    i++;
    linelength++;
  }

  s[i] = '\0';
  return linelength;
}

// copy: copy 'from' into 'to'; assume 'to' is big enough
void copy(char to[], char from[]) {
  int i = 0;

  while ((to[i] = from[i]) != '\0') {
    i++;
  }
}
