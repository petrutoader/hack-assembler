#include <stdio.h>

void main() {
  FILE *fh;

  fh = fopen("Add.asm", "r");
  char ch = getc(fp);

  printf("%c", ch);
  fclose(fp);
}
