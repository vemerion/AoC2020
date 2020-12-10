#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 16

typedef struct {
  unsigned *numbers;
  size_t index;
  size_t size;
} alist_t;

static int cmp(const void *a, const void *b)
{
  const unsigned *x = a;
  const unsigned *y = b;

  if (*x < *y)
    return -1;
  else if (*x == *y)
    return 0;
  else
    return 1;
}

static unsigned chain_differences(unsigned numbers[], size_t length)
{
  unsigned one_diff_count, three_diff_count, diff;

  one_diff_count = 0;
  three_diff_count = 0;


  for (size_t i = 1; i < length; i++) {
    diff = numbers[i] - numbers[i - 1];
    if (diff == 1)
      one_diff_count++;
    else if (diff == 3)
      three_diff_count++;
  }
  return one_diff_count * three_diff_count;
}

static unsigned trib(unsigned n)
{
  if (n == 0)
    return 0;
  else if (n == 1 || n == 2)
    return 1;
  else 
    return trib(n - 1) + trib(n - 2) + trib(n - 3);
}

static unsigned long long total_combinations(unsigned numbers[], size_t length)
{
  unsigned long long combinations = 1;
  unsigned remove_chain_length = 1;

  for (size_t i = 1; i < length; i++)
    if (numbers[i] - numbers[i - 1] == 1) {
      remove_chain_length++;
    } else {
      combinations *= trib(remove_chain_length);
      remove_chain_length = 1;
    }

  return combinations;
}

static void add(alist_t *alist, unsigned n)
{
  if (alist->index == alist->size) {
    alist->size *= 2;
    alist->numbers = realloc(alist->numbers, alist->size * sizeof(unsigned));
  }
  alist->numbers[alist->index++] = n;
}

int main(void)
{
  alist_t *alist = calloc(1, sizeof(alist_t));
  alist->numbers = malloc(INITIAL_SIZE * sizeof(unsigned));
  unsigned n, largest;

  largest = 0;
  alist->size = INITIAL_SIZE;

  while (scanf("%u", &n) != EOF) {
    add(alist, n);
    if (n > largest)
      largest = n;
  }
  add(alist, 0);
  add(alist, largest + 3);

  qsort(alist->numbers, alist->index, sizeof(unsigned), cmp);

  printf("Part 1 answer = %u\n", chain_differences(alist->numbers, alist->index));
  printf("Part 2 answer = %llu\n", total_combinations(alist->numbers, alist->index));
  free(alist->numbers);
  free(alist);
  return 0; 
}
