#include <stdio.h>
#include <stdbool.h>

bool part1_valid(char *password, unsigned min, unsigned max, char c)
{
  char *p = password;
  unsigned char_count = 0;
  for (p = password; *p; p++)
    if (*p == c)
      char_count++;
  if (char_count >= min && char_count <= max)
    return true;
  return false;
}

bool part2_valid(char *password, unsigned pos1, unsigned pos2, char c)
{
  bool pos1_correct = password[pos1 - 1] == c;
  bool pos2_correct = password[pos2 - 1] == c;
  return (pos1_correct || pos2_correct) && !(pos1_correct && pos2_correct);
}

int main(void)
{
  unsigned min, max, part1_valid_count, part2_valid_count;
  char c;
  char password[BUFSIZ];

  part1_valid_count = part2_valid_count = 0;

  while (scanf("%u-%u %c: %s", &min, &max, &c, password) != EOF) {
    if (part1_valid(password, min, max, c))
      part1_valid_count++;
    if (part2_valid(password, min, max, c))
      part2_valid_count++;
  }
  printf("Part 1 result = %u\n", part1_valid_count);
  printf("Part 2 result = %u\n", part2_valid_count);
}
