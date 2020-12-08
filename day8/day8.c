#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define START_SIZE 16
#define EXPAND_RATE 2

typedef enum { NOP, ACC, JMP } instype_t;

typedef struct {
  instype_t type;
  int arg;
  bool executed;
} instr_t;

typedef struct {
  size_t index;
  size_t size;
  instr_t *instrs;
} prog_t;

static void expand_prog(prog_t *prog)
{
  prog->size *= EXPAND_RATE;
  prog->instrs = realloc(prog->instrs, prog->size * sizeof(instr_t));
}

static void add_instr(prog_t *prog, instype_t type, int arg)
{
  if (prog->index == prog->size)
    expand_prog(prog);

  prog->instrs[prog->index++] = (instr_t) { .type = type, .arg = arg };
}

static void init_prog(prog_t **prog)
{
  prog_t *p;

  *prog = calloc(1, sizeof(prog_t));
  p = *prog;
  p->size = START_SIZE;
  p->instrs = calloc(p->size, sizeof(instr_t));
}

static instype_t strtotype(char s[])
{
  if (!strcmp(s, "acc")) {
    return ACC;
  } else if (!strcmp(s, "nop")) {
    return NOP;
  } else if (!strcmp(s, "jmp")) {
    return JMP;
  }
  return NOP;
}

static void clear_prog(prog_t *prog)
{
  for (size_t i = 0; i < prog->index; i++)
    prog->instrs[i].executed = false;
}

// Returns true if no infinite loop
static bool run(prog_t *prog, int *result)
{
  size_t pc;
  int acc;
  
  pc = 0;
  acc = 0;

  clear_prog(prog);

  while (!(pc == prog->index)) {
    if (prog->instrs[pc].executed) {
      *result = acc;
      return false;
    }
    prog->instrs[pc].executed = true;
    switch (prog->instrs[pc].type) {
      case NOP:
        break;
      case ACC:
        acc += prog->instrs[pc].arg;
        break;
      case JMP:
        pc += prog->instrs[pc].arg;
      continue;
    }
    pc++;
  }
  *result = acc;
  return true;
}

static void change_instr(instr_t *inst)
{
  if (inst->type == NOP)
    inst->type = JMP;
  else if (inst->type == JMP)
    inst->type = NOP;
}

static void run_change(prog_t *prog, int *result)
{
  for (size_t i = 0; i < prog->index; i++) {
    change_instr(&prog->instrs[i]);
    if (run(prog, result))
      return;
    change_instr(&prog->instrs[i]);
  }
}

int main(void)
{
  char type[BUFSIZ];
  int arg, result_part1, result_part2;
  prog_t *prog = NULL;

  init_prog(&prog);

  while (scanf("%s %d", type, &arg) != EOF) {
    add_instr(prog, strtotype(type), arg);
  }
  
  run(prog, &result_part1);
  printf("Part 1 answer = %d\n", result_part1);
  run_change(prog, &result_part2);
  printf("Part 2 answer = %d\n", result_part2);
  free(prog->instrs);
  free(prog);
  return 0;
}
