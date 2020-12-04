#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define FIELD_COUNT 8
#define EYECOLOR_COUNT 7

static char KEYS[][4] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid" };
static char EYECOLORS[][4] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

static bool valid_number(char *value, unsigned min, unsigned max)
{
  unsigned number;
  if (sscanf(value, "%u", &number) == 1) {
    return number >= min && number <= max;
  }
  return false;
}

static bool valid_height(char *value)
{
  unsigned number;
  char unit[BUFSIZ];
  
  if (sscanf(value, "%u%s", &number, unit) == 2) {
    if (!strcmp(unit, "cm"))
      return number >= 150 && number <= 193;
    else if (!strcmp(unit, "in"))
      return number >= 59 && number <= 76;
  }
  return false;
}

static bool valid_haircolor(char *value)
{
  size_t i = 0;
  while (*value) {
    if (i == 0 && *value != '#')
      return false;
    else if (i != 0 && !(isdigit(*value) || (*value >= 'a' && *value <= 'f')))
      return false;
    value++;
    i++;
  }
  return i == 7;
}

static bool valid_eyecolor(char *value)
{
  for (size_t i = 0; i < EYECOLOR_COUNT; i++) {
    if (!strcmp(value, EYECOLORS[i]))
      return true;
  }
  return false;
}

static bool valid_pid(char *value)
{
  size_t i = 0;
  while (*value) {
    if (!isdigit(*value))
      return false;
    value++;
    i++;
  }
  return i == 9;
}

static bool valid_value(char *key, char *value)
{
  if (!strcmp("byr", key)) {
      return valid_number(value, 1920, 2002);
  } else if (!strcmp("iyr", key)) {
    return valid_number(value, 2010, 2020);
  } else if (!strcmp("eyr", key)) {
    return valid_number(value, 2020, 2030);
  } else if (!strcmp("hgt", key)) {
    return valid_height(value);
  } else if (!strcmp("hcl", key)) {
    return valid_haircolor(value);
  } else if (!strcmp("ecl", key)) {
    return valid_eyecolor(value);
  } else if (!strcmp("pid", key)) {
    return valid_pid(value);
  }  
  return false;
}

static bool valid_field(char *key, char *value)
{
  for (size_t i = 0; i < FIELD_COUNT - 1; i++)
    if (!strcmp(KEYS[i], key))
      return valid_value(key, value);;
  return false;
}

static bool valid_key(char *key)
{
  for (size_t i = 0; i < FIELD_COUNT - 1; i++)
    if (!strcmp(KEYS[i], key))
      return true;
  return false;
}

int main(void)
{
  int c, prev;
  bool after_colon;
  char key[BUFSIZ], value[BUFSIZ];
  unsigned valid_count_part1, valid_count_part2, valid_fields, valid_keys;
  size_t key_index, value_index;

  valid_count_part1 = valid_count_part2 = valid_fields = valid_keys = 0;
  key_index = value_index = 0;
  after_colon = false;
  prev = '\0';
  
  while ((c = getchar()) != EOF) {
    if (c == ':') {
      after_colon = true;
    } else if (c == '\n' || c == ' ') {
      if (c == '\n' && prev == '\n') {
        if (valid_keys == FIELD_COUNT - 1)
          valid_count_part1++;
        if (valid_fields == FIELD_COUNT - 1)
          valid_count_part2++;
        valid_fields = 0;
        valid_keys = 0;
      } else {
        value[value_index] = key[key_index] = '\0';
        value_index = key_index = 0;
        if (valid_field(key, value))
          valid_fields++;
        if (valid_key(key))
          valid_keys++;
        if (c == ' ')
          after_colon = false;
      }
      after_colon = false;
    } else {
      if (after_colon)
        value[value_index++] = c;
      else
        key[key_index++] = c;
    }
    prev = c;
  }  
  if (valid_keys == FIELD_COUNT - 1)
    valid_count_part1++;
  if (valid_fields == FIELD_COUNT - 1)
    valid_count_part2++;
  printf("Part 1 answer = %u\n", valid_count_part1);
  printf("Part 2 answer = %u\n", valid_count_part2);
  return 0;
}
