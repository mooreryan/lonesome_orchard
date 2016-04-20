#include <stdio.h>
#include <string.h>

#define MAXLINE 100

void copy_into_header(char *header, char *line)
{
  /* skip the first char in line, it's the @ we don't want */
  *++line;

  /* copy the rest */
  while((*header++ = *line++)) {
    ;
  }
}

/* read line into line, and return length of line, strips newline
   chars at end of line. returns -1 on EOF, 0 if line is just a '\n',
   length otherwise */
int readline(char line[], int size)
{
  int c = 0;
  int i = 0;

  for (i = 0; i < size - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    line[i] = c;
  }

  line[i] = '\0';

  if (c == EOF) {
    return -1;
  } else {
    return i;
  }
}

int read_record(char head[], char seq[], char qual[], int size)
{
  int len = 0;
  int lineno = 0;
  char line[size];

  while ((len = readline(line, size)) >= 0) {
    if (len > 0) {
      switch (lineno) {
      case 0:
        copy_into_header(head, line);
        break;
      case 1:
        strcpy(seq, line);
        break;
      case 3:
        strcpy(qual, line);
        return len;
      }

      ++lineno;
    }
  }

  return -1;
}

int main(int argc, char* argv[])
{
  int len = 0;
  char head[MAXLINE];
  char seq[MAXLINE];
  char qual[MAXLINE];

  while ((len = read_record(head, seq, qual, MAXLINE)) >= 0) {

    if (len == -1) {
      fprintf(stderr, "ERROR -- problem in read_record\n");
      return 1;
    }

    printf("@%s\n", head);
    printf("%s\n", seq);
    puts("+");
    printf("%s\n", qual);
  }

  return 0;
}
