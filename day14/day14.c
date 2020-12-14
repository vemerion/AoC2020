#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 128
#define BITS 36

typedef struct node_t node_t;
typedef unsigned long long number_t;

struct node_t {
  node_t *next;
  number_t addr;
  number_t value;
};

static node_t *create_node(number_t addr, number_t value)
{
  node_t *n = calloc(1, sizeof(node_t));
  n->addr = addr;
  n->value = value;
  return n;
}

static void insert_value(node_t *mem[], number_t addr, number_t value)
{
  node_t *node;
  number_t index = addr % N;

  if (mem[index] == NULL) {
    mem[index] = create_node(addr, value);
  } else {
    node = mem[index];
    while (node->addr != addr && node->next != NULL)
      node = node->next;

    if (node->addr == addr)
      node->value = value;
    else 
      node->next = create_node(addr, value);
  }
}

static number_t mask(number_t value, char bitmask[])
{
  for (size_t i = 0; i < BITS; i++)
    switch (bitmask[BITS - 1 - i]) {
      case '0':
        value &= ~((number_t)1 << i);
        break;
      case '1':
        value |= (number_t)1 << i;
        break;
      default:
        break;
    }
  return value;
}

static void insert_value_floating(node_t *mem[], number_t addr, number_t value, char bitmask[], size_t bitmask_index)
{
  for (size_t i = bitmask_index; i < BITS; i++)
    switch (bitmask[BITS - 1 - i]) {
      case '0':
        break;
      case '1':
        addr |= (number_t)1 << i;
        break;
      case 'X':
        insert_value_floating(mem, addr & ~((number_t)1 << i), value, bitmask, i + 1);
        insert_value_floating(mem, addr | ((number_t)1 << i), value, bitmask, i + 1);
        return;
      default:
        break;
    }
  insert_value(mem, addr, value);
}

static number_t sum(node_t *mem[])
{
  node_t *n;
  number_t sum = 0;

  for (size_t i = 0; i < N; i++) {
    n = mem[i];
    while (n != NULL) {
      sum += n->value;
      n = n->next;
    }
  }
  return sum;
}

static void free_mem(node_t *mem[])
{
  node_t *n;
  for (size_t i = 0; i < N; i++) {
    n = mem[i];
    while (n != NULL) {
      n = n->next;
      free(mem[i]);
      mem[i] = n;
    }
  }
}

int main(void)
{
  number_t addr, value;
  char bitmask[BITS + 1], string[BUFSIZ];
  node_t *mem[N] = { 0 };
  node_t *mem2[N] = { 0 };
  
  while (scanf("%[^[ ]", string) != EOF) {
    if (!strcmp(string, "mask")) {
      scanf(" = %s\n", bitmask);
    } else if (!strcmp(string, "mem")) {
      scanf("[%llu] = %llu\n", &addr, &value);
      insert_value(mem, addr, mask(value, bitmask));
      insert_value_floating(mem2, addr, value, bitmask, 0);
    }
  }
  printf("Part 1 answer = %llu\n", sum(mem));
  printf("Part 2 answer = %llu\n", sum(mem2));

  free_mem(mem);
  free_mem(mem2);
  return 0;
}
