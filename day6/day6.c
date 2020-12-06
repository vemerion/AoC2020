#include <stdio.h>
#include <stdbool.h>

#define QUESTION_COUNT 26

static int question_index(int c)
{
  return c - 'a';
}

static unsigned yes_count_part1(unsigned questions[], bool reset)
{
  unsigned count = 0;
  for (size_t i = 0; i < QUESTION_COUNT; i++) {
    count += questions[i] > 0;
    if (reset)
      questions[i] = 0;
  }
  return count;
}

static unsigned yes_count_part2(unsigned questions[], unsigned group_size, bool reset)
{
  unsigned count = 0;
  for (size_t i = 0; i < QUESTION_COUNT; i++) {
    if (questions[i] == group_size)
      count += questions[i] > 0;
    if (reset)
      questions[i] = 0;
  }
  return count;
}

int main(void)
{
  int c, prev;
  unsigned questions[QUESTION_COUNT] = { 0 };
  unsigned total_yes_part1, total_yes_part2, group_size;

  prev = '\0';
  total_yes_part1 = total_yes_part2 = group_size = 0;

  while (true) {
    c = getchar();
    if (c == '\n' || c == EOF) {
      if (prev == '\n') {
        total_yes_part1 += yes_count_part1(questions, false);
        total_yes_part2 += yes_count_part2(questions, group_size, true);
        group_size = 0;
        if (c == EOF)
          break;
      } else {
        group_size++;
      }
    } else {
      questions[question_index(c)]++;
    }
    prev = c;
  }

  printf("Part 1 answer = %u\n", total_yes_part1);
  printf("Part 2 answer = %u\n", total_yes_part2);
  
  return 0;
}
