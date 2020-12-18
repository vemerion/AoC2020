#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct op_t op_t;

typedef enum { NUM, MUL, ADD } type_t;

struct op_t {
  type_t type;
  int num;
  op_t *lhs, *rhs;
};

static char line[BUFSIZ];
static size_t index;

static char next_char()
{
  return line[index++];
}

static op_t *parse_next();
static op_t *parse_op(bool);

static op_t *parse_next() {
  char c;
  op_t *op;

  while (true) {
    c = next_char();
    if (isdigit(c)) {
      op = calloc(1, sizeof(op_t));
      op->type = NUM;
      op->num = c - '0';
      return op;
    } else if (c == '(')
      return parse_op(true);
  }
  return NULL;
}

static op_t *parse_op(bool part2)
{
  char c;
  op_t *res, *op, *temp, *temp2;
  
  res = op = NULL;
  while (true) {
    c = next_char();
    if (c == '\n' || c == ')') {
      res->rhs = op;
      return res;
    } else if (isdigit(c)) {
      op = calloc(1, sizeof(op_t));
      op->type = NUM;
      op->num = c - '0';
    } else if (c == '+' || c == '*') {
      if (res == NULL) {
        res = calloc(1, sizeof(op_t));
        res->lhs = op;
        res->type = c == '+' ? ADD : MUL;
      } else {
        if (c == '+' && res->type == MUL && part2) {
          temp = parse_next();
          temp2 = op;
          op = calloc(1, sizeof(op_t));
          op->type = ADD;
          op->lhs = temp2;
          op->rhs = temp;
          continue;
        }
        res->rhs = op;
        temp = calloc(1, sizeof(op_t)); 
        temp->lhs = res;
        temp->type = c == '+' ? ADD : MUL;
        res = temp;
        op = NULL;
      }
    } else if (c == '(') {
      op = parse_op(part2);
    }
  }
  return res;
}

static long long value(op_t *op)
{
  switch (op->type) {
    case NUM:
      return op->num;
    case MUL:
      return value(op->lhs) * value(op->rhs);
    case ADD:
      return value(op->lhs) + value(op->rhs);
    default:
      return -1;
  }
}

static void free_op(op_t *op)
{
  switch (op->type) {
    case NUM:
      free(op);
      break;
    case MUL:
    case ADD:
      free_op(op->lhs);
      free_op(op->rhs);
      free(op);
      break;
    default:
      break;
  }
}


int main(void)
{
  op_t *op;
  long long sum_part1, sum_part2;

  sum_part1 = sum_part2 = 0;

  while (fgets(line, BUFSIZ, stdin) != NULL) {
    index = 0;
    op = parse_op(false);
    sum_part1 += value(op);
    free_op(op);
    index = 0;
    op = parse_op(true);
    sum_part2 += value(op);
    free_op(op);
  }

  printf("Part 1 answer = %lld\n", sum_part1);
  printf("Part 2 answer = %lld\n", sum_part2);
  return 0;
}
