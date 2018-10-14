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

char compTable[8][12] = {
  {'0',                   '0',      '1', '0', '1', '0', '1', '0'},
  {'1',                   '0',      '1', '1', '1', '1', '1', '1'},
  {'-', '1',              '0',      '1', '1', '1', '0', '1', '0'},
  {'D',                   '0',      '0', '0', '1', '1', '0', '0'},
  {'A',                   '0',      '1', '1', '0', '0', '0', '0'},
  {'M',                   '1',      '1', '1', '0', '0', '0', '0'},
  {'!', 'D',              '0',      '0', '0', '1', '1', '0', '1'},
  {'D', '-', 'M', 'A',    '0',      '1', '0', '1', '0', '1', '0'},
};

void getBin(int num, char *str) {
  *(str+16) = '\0';
  int mask = 0x1000 << 4;
  while(mask >>= 1)
    *str++ = !!(mask & num) + '0';
}

void getCompBin(char *compString, char *compBin) {
  for (int compRow = 0; compRow < 8; compRow++) {
    if (strncmp(compTable[compRow], compString, strlen(compString)) == 0) {

      for(int i = strlen(compString); i < strlen(compTable[compRow]); i++) {
        compBin[i - strlen(compString)] = compTable[compRow][i];
      }

      break;
    };
  }
}

char * transformCCommand(char *line, char *binaryRepresentation) {
  size_t linelength = strlen(line);

  char compString[4] = { 0 };
  char compBin[7] = { 0 };

  char destString[3] = { 0 };
  char jmpString[3] = { 0 };

  /* for (int position = 0; position < linelength; position++) { */
  /*   if (line[position] == '=') { */
  /*     break; */
  /*   } else { */
  /*     compString[position] = line[position]; */
  /*   } */
  /* } */

  getCompBin(compString, compBin);

  for (int bit = 0; bit <= 2; bit++) {
    binaryRepresentation[bit] = '1';
  }

  for (int bit = 3; bit <= 9; bit++) {
    binaryRepresentation[bit] = 'c';
  }

  for (int bit = 10; bit <= 12; bit++) {
    binaryRepresentation[bit] = 'd';
  }

  for (int bit = 13; bit <= 15; bit++) {
    binaryRepresentation[bit] = 'j';
  }

  return binaryRepresentation;
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
  } else {
    transformCCommand(line, binaryRepresentation);
  }

  return binaryRepresentation;
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
    char binaryRepresentation[17] = { 0 };

    // If it's an empty line or starts with "//", then ignore it.
    if (!isspace(*line) && (line[0] != '/') && (line[1] != '/')) {
        assemble(line, binaryRepresentation);
        printf("%s\n", binaryRepresentation);
    }
  }

  fclose(srcFile);
  return 0;
}
