#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define PREAMBLE_COUNT 25
#define NUMBER_COUNT 1000

static bool issum(unsigned long long preamble[], unsigned long long n)
{
  for (size_t i = 0; i < PREAMBLE_COUNT; i++) {
    for (size_t j = 0; j < PREAMBLE_COUNT; j++) {
      if (preamble[i] != preamble[j] && preamble[i] + preamble[j] == n)
        return true;
    }
  }
  return false;
}

static unsigned long long find_contiguous_range(unsigned long long numbers[], unsigned long long invalid)
{
  unsigned long long smallest, largest, sum, n;

  
  for (size_t i = 0; i < NUMBER_COUNT; i++) {
    sum = 0;
    smallest = ULLONG_MAX;
    largest = 0;
    for (size_t j = i; j < NUMBER_COUNT; j++) {
      n = numbers[j];
      sum += n;
      if (n < smallest)
        smallest = n;
      if (n > largest)
        largest = n;
      if (sum == invalid)
        return smallest + largest;
      if (sum > invalid)
        break;
    }
  }
  return 0;
}

int main(void)
{
  unsigned long long preamble[PREAMBLE_COUNT], numbers[NUMBER_COUNT];
  unsigned long long n, invalid;
  size_t i = 0;

  invalid = 0;

  while (scanf("%llu", &n) != EOF) {
    if (i >= PREAMBLE_COUNT)
      if(!issum(preamble, n) && invalid == 0)
        invalid = n;
    preamble[i % PREAMBLE_COUNT] = n;
    numbers[i] = n;
    i++;
  }
  printf("Part 1 answer = %llu\n", invalid);
  printf("Part 2 answer = %llu\n", find_contiguous_range(numbers, invalid));

  return 0;
}
