#include <stdio.h>
#include <stdlib.h>

#define N 2097152

typedef struct node_t node_t;

struct node_t {
  node_t *next;
  unsigned number;
  unsigned last_seen;
};

static node_t *create_node(unsigned number, unsigned last_seen)
{
  node_t *node = calloc(1, sizeof(node_t));
  node->number = number;
  node->last_seen = last_seen;
  return node;
}

static unsigned speak(node_t *numbers[], unsigned number, unsigned turn)
{
  node_t *n;
  unsigned index = number % N;
  if (numbers[index] == NULL) {
    numbers[index] = create_node(number, turn);
  } else {
    n = numbers[index];
    while (n->number != number && n->next != NULL)
      n = n->next;
    
    if (n->number == number) {
      unsigned ret = turn - n->last_seen;
      n->last_seen = turn;
      return ret;
    } else
      n->next = create_node(number, turn);
  }
  return 0;
}

static void free_numbers(node_t *numbers[], size_t n)
{
  node_t *node;
  for (size_t i = 0; i < n; i++) {
    node = numbers[i];
    while (node != NULL) {
      node = node->next;
      free(numbers[i]);
      numbers[i] = node;
    }
  }
}

int main(void)
{
  unsigned number, response, turn;
  node_t **numbers = calloc(N, sizeof(node_t*));
  
  turn = response = 1;
  while (scanf("%u,", &number) != EOF) {
    response = speak(numbers, number, turn);
    turn++;
  }

  while (turn != 2020) {
    response = speak(numbers, response, turn);
    turn++;
  }

  printf("Part 1 answer = %u\n", response);

  while (turn != 30000000) {
    response = speak(numbers, response, turn);
    turn++;
  }

  printf("Part 2 answer = %u\n", response);
    
  free_numbers(numbers, N);
  free(numbers);
  return 0;
}
