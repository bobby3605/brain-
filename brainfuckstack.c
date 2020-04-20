#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

void runner();

unsigned int ramindex = 256;
unsigned int ram[1048832] = {0};
FILE *pc;

struct Stack {
  unsigned int top;
  unsigned int* array;
};

struct Stack* loop;

unsigned int ss = 0;

void push(struct Stack* stack, unsigned int item){
  stack->array[++stack->top] = item;
  ss++;
}

unsigned int pop(struct Stack* stack){
  ss--;
  return stack->array[stack->top--];
}

int main(int argc, char *argv[]) {
 loop = (struct Stack*)malloc(sizeof(struct Stack));
 loop->top = -1;
 loop->array = (unsigned int*)malloc(4000000*sizeof(unsigned int));
  if (argc != 2) {
    printf("Not 1 argument\n");
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
  char c = fgetc(pc);
  switch (c) {
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
    if(ram[ramindex] == 0){
      while(fgetc(pc) != ']'){}
    }
    push(loop, ftell(pc));
    break;
  case ']':
    if (ram[ramindex] != 0) {
      // -1 because fgetc(pc) gets the next character
      int popped = pop(loop)-1;
      fseek(pc,popped,SEEK_SET);
      return;
    }
    // Pops the ] pc off the stack when hitting 0
    pop(loop);
    break;
  }
}
