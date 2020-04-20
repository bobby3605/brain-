#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

void runner();

unsigned int ramindex = 1000;
unsigned int ram[1000000] = {0};
FILE *pc;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: path to bainfuck file\n");
    exit(0);
  }

  pc = fopen(argv[1], "r");
  if (pc == NULL) {
    printf("Error opening file\n");
    exit(0);
  }
  while (!feof(pc)) {
    runner();
  }
}

void runner() {
  switch (fgetc(pc)) {
  case '>':
    ++ramindex;
    break;
  case '<':
    --ramindex;
    break;
  case '+':
    ++ram[ramindex];
    break;
  case '-':
    --ram[ramindex];
    break;
  case '.':
    printf("%c", ram[ramindex]);
    break;
  case ',':
    ram[ramindex] = getchar();
    break;
  case '[':
    if (ram[ramindex] == 0) {
      // Jump to matching ]
      while (fgetc(pc) != ']') {
      }
      break;
    case ']':
      if (ram[ramindex] != 0) {
        // Counts to find matching [
        int count = 1;
        // Seek 2 backwards because fgetc() gets the next character
        // Want 1 character behind
        fseek(pc, -2, SEEK_CUR);
        char c;
        while ((c = fgetc(pc))) {
          // Seek, but in the loop
          fseek(pc, -2, SEEK_CUR);
          switch (c) {
          case ']':
            // Increase count if not matched
            count++;
            break;
          case '[':
            // If count == 0, the [] has been matched correctly
            count--;
            if (count == 0) {
              // Seek 1 forwards to undo the -2 seek
              // The next fgetc() should return [
              fseek(pc, 1, SEEK_CUR);
              return;
            }
            break;
          }
        }
      }
      break;
    }
  }
}
