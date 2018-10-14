#include <ctype.h>
#include <limits.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

#define IDLE -1;
#define READ_STATE 0
#define COMMENT_STATE 1
#define CPU_BITS 16

int state = IDLE;

void getBin(int num, char *str) {
  *(str+16) = '\0';
  int mask = 0x1000 << 4;
  while(mask >>= 1)
    *str++ = !!(mask & num) + '0';
}

char * transformACommand(char *line, char *binaryRepresentation) {
  char substring[16];
  size_t linelength = strlen(line);

  for (int length = 1; length < linelength; length++) {
    substring[length - 1] = line[length];
  }

  getBin(atoi(substring), binaryRepresentation);

  return binaryRepresentation;
}


char *assemble(char *line, char *binaryRepresentation) {
  if (line[0] == '@') {
    transformACommand(line, binaryRepresentation);
  }

  return binaryRepresentation;
}

void removeCommentAndEmptyLines(FILE *file) {
}

int main() {
  FILE *srcFile;

  srcFile = fopen("Add.asm", "r");

  if(srcFile == NULL) {
    exit(EXIT_FAILURE);
  }
  state = READ_STATE;

  char line[BUFFER_SIZE];

  while((fgets(line, BUFFER_SIZE, srcFile)) != NULL) {
    // If it's an empty line or starts with "//", then ignore it.
    if (!isspace(*line) && (line[0] != '/') && (line[1] != '/')) {
        char binaryRepresentation[16] = "";
        assemble(line, binaryRepresentation);
        printf("%s\n", binaryRepresentation);
    }
  }

  fclose(srcFile);
  return 0;
}
