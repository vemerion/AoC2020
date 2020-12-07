#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000

typedef struct entry_t entry_t;
typedef struct list_t list_t;

struct entry_t {
  char name[20];
  list_t *subbags;
  entry_t *next;
};

struct list_t {
  char name[20];
  unsigned count;
  list_t *next;
};

static size_t hash(char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

static void add_rule(entry_t *rules[], entry_t *entry)
{
  size_t h = hash(entry->name) % TABLE_SIZE;
  entry_t *e = rules[h];
  if (e == NULL) {
    rules[h] = entry;
  } else {
    while (e->next != NULL)
      e = e->next;
    e->next = entry;
  }
}

static void add_subbag(entry_t *entry, list_t* list)
{
  list_t *l = entry->subbags;

  if (entry->subbags == NULL)
    entry->subbags = list;
  else {
    l = entry->subbags;
    while (l->next != NULL)
      l = l->next;
    l->next = list;
  }
}

static bool ends_with_dot(char s[])
{
  return s[strlen(s) - 1] == '.';
}

static bool can_contains_shiny_gold(entry_t *rules[], entry_t *entry);

static unsigned total_contain_shiny_gold(entry_t *rules[])
{
  entry_t *entry;
  unsigned total = 0;
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    entry = rules[i];
    while (entry != NULL) {
      total += can_contains_shiny_gold(rules, entry);
      entry = entry->next;
    }
  }
  return total;
}

static entry_t *get(entry_t *rules[], char name[])
{
  entry_t *entry = rules[hash(name) % TABLE_SIZE];
  while (strcmp(entry->name, name))
    entry = entry->next;
  return entry;
}


static bool can_contains_shiny_gold(entry_t *rules[], entry_t *entry)
{
  list_t *list = entry->subbags;
  while (list != NULL) {
    if (!strcmp(list->name, "shinygold"))
      return true;
    else {
      if (can_contains_shiny_gold(rules, get(rules, list->name)))
        return true;
    }
    list = list->next;
  }
  return false;
}

static unsigned total_bag_count(entry_t *rules[], entry_t *entry, unsigned multiplier)
{
  unsigned count = multiplier;
  list_t *list = entry->subbags;

  while (list != NULL) {
    count += total_bag_count(rules, get(rules, list->name), multiplier * list->count);
    list = list->next;
  }
  return count;
}

static unsigned total_bag_count_in_shiny_gold_bag(entry_t *rules[])
{
  return total_bag_count(rules, get(rules, "shinygold"), 1) - 1; // Subtract one to avoid counting the gold bag itself
}

static void free_list(list_t *list)
{
  list_t *l = list;
  while (l != NULL) {
    list = list->next;
    free(l);
    l = list;
  }
}

static void free_entries(entry_t *entry)
{
  entry_t *e = entry;
  while (e != NULL) {
    free_list(e->subbags);
    entry = entry->next;
    free(e);
    e = entry;
  }
}

static void free_table(entry_t *rules[])
{
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    free_entries(rules[i]);
  }
}

int main(void)
{
  char w1[BUFSIZ], w2[BUFSIZ], w3[BUFSIZ];
  entry_t *rules[TABLE_SIZE] = { 0 };
  entry_t *entry;
  list_t *list;
  unsigned count;

  while (scanf("%s %s %*s %*s", w1, w2) != EOF) {
    strcat(w1, w2);
    entry = calloc(1, sizeof(entry_t));
    strcpy(entry->name, w1);
    while (true) {
      scanf("%s", w1);
      if (!strcmp(w1, "no")) {
        scanf("%*s %*s");
        break;
      }
      
      sscanf(w1, "%u", &count);
      scanf("%s %s %s", w1, w2, w3);
      strcat(w1, w2);
      list = calloc(1, sizeof(list_t));
      strcpy(list->name, w1);
      list->count = count;
      add_subbag(entry, list);
      if (ends_with_dot(w3))
        break;
    }
    add_rule(rules, entry);
  }
  printf("Part 1 answer = %u\n", total_contain_shiny_gold(rules));
  printf("Part 2 answer = %u\n", total_bag_count_in_shiny_gold_bag(rules));
  free_table(rules);
  return 0;
}
