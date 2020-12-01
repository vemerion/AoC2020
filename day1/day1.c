#include <stdio.h>
#include <stdlib.h>

#define START_SIZE 16
#define EXPAND_RATE 2

typedef struct {
  unsigned *numbers;
  size_t index;
  size_t size;
} expenses_t;

static void expand_expenses(expenses_t *expenses)
{
  expenses->size *= EXPAND_RATE;
  expenses->numbers = realloc(expenses->numbers, expenses->size * sizeof(unsigned));
}

static void init_expenses(expenses_t **expenses)
{
  expenses_t *p;
  *expenses = malloc(sizeof(expenses_t));
  p = *expenses;
  p->index = 0;
  p->size = START_SIZE;
  p->numbers = malloc(p->size * sizeof(unsigned));
}

static void add_expense(expenses_t **expenses, unsigned expense)
{
  expenses_t *p;
  if (*expenses == NULL)
    init_expenses(expenses);

  p = *expenses;
  if (p->index == p->size)
    expand_expenses(p);
  
  p->numbers[p->index++] = expense;
}

int main(void)
{
  unsigned number;
  size_t i, j, k, count;
  expenses_t *expenses = NULL;
  unsigned *numbers;

  while (scanf("%u\n", &number) != EOF) {
    add_expense(&expenses, number);
    for (i = 0; i < expenses->index; i++)
      if (number + expenses->numbers[i] == 2020)
        printf("part 1 result = %u\n", number * expenses->numbers[i]);
  }

  count = expenses->index;
  numbers = expenses->numbers;
  for (i = 0; i < count; i++)
    for (j = 0; j < count; j++)
      for (k = 0; k < count; k++)
        if (numbers[i] + numbers[j] + numbers[k] == 2020) {
          printf("part 2 result = %u\n", numbers[i] * numbers[j] * numbers[k]);
          goto end;
        }

end:
  free(expenses->numbers);
  free(expenses);
  return 0;
}
