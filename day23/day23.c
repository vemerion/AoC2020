#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct list_t list_t;

struct list_t {
  int n;
  list_t *next;
};

static void print_list(list_t *list)
{
  list_t* l = list;

  if (list == NULL)
    return;

  do {
    printf("%d ", l->n);
    l = l->next;
  } while (list != l);

  printf("\n");
}

static list_t *find_destination(list_t *current)
{
  int n = current->n;

  while (n == current->n || n == current->next->n || n == current->next->next->n || n == current->next->next->next->n) {
    n--;
    if (n == 0)
      n = 9;
  }

  while (current->n != n)
    current = current->next;

  return current;
}

static void crab_cups(list_t *current)
{
  list_t *destination, *picked;

  for (size_t i = 0; i < 100; i++) {
    destination = find_destination(current);
    picked = current->next;
    current->next = picked->next->next->next;
    picked->next->next->next = destination->next;
    destination->next = picked;
    current = current->next;
  }
}

static long result(list_t *list)
{
  long result;
  list_t *one;

  result = 0;

  while (list->n != 1)
    list = list->next;
  one = list;
  list = list->next;

  do {
    result = result * 10 + list->n;
    list = list->next;
  } while (list != one); 

  return result;
}

static void free_list(list_t *list)
{
  list_t *l, *h;

  l = h = list;

  do {
    l = l->next;
    free(list);
    list = l;
  } while (l != h);
}

int main(void)
{
  list_t *h, *t;
  int c;

  h = t = NULL;
  
  while ((c = getchar()) != EOF) {
    if (isdigit(c)) {
      if (h == NULL) {
        h = calloc(1, sizeof(list_t));
        h->n = c - '0';
        t = h;
      } else {
        t->next = calloc(1, sizeof(list_t));
        t->next->n = c - '0';
        t = t->next;
      }
    }
  }
  t->next = h;

  crab_cups(h);
  print_list(h);
  printf("Part 1 answer = %ld\n", result(h));
  free_list(h);
  return 0;
}
