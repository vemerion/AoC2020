#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct list_t list_t;
typedef struct food_t food_t;

struct food_t {
  food_t *next;
  list_t *ingredients;
  list_t *allergens;
};

struct list_t {
  char *string;
  list_t *next;

  list_t *ingredients; // Only used for allergens, the list of possible foods that can contain allergen
};

static size_t len(list_t *l)
{
  size_t n = 0;

  while (l != NULL) {
    n++;
    l = l->next;
  }
  return n;
}

static void add_list(list_t **list, char *s)
{
  list_t *l = calloc(1, sizeof(list_t));
  l->string = malloc(strlen(s) + 1);
  strcpy(l->string, s);
  l->next = *list;
  *list = l;
}

static void remove_list(list_t **list, char *s)
{
  list_t *l, *m;
  l = *list;

  if (l == NULL)
    return;
  
  if (!strcmp(l->string, s)) {
    m = l->next;
    free(l->string);
    free(l);
    *list = m;
    return;
  }

  while (l->next != NULL) {
    if (!strcmp(l->next->string, s)) {
      m = l->next->next;
      free(l->next->string);
      free(l->next);
      l->next = m;
      return;
    }
    l = l->next;
  }
}

static bool contains(list_t *list, char *s)
{
  while (list != NULL) {
    if (!strcmp(list->string, s))
      return true;
    list = list->next;
  }
  return false;
}

static food_t *parse_food(char *line)
{
  food_t *food = calloc(1, sizeof(food_t));
  char word[BUFSIZ] = { 0 };
  size_t n;
  bool after_contains;
  char c;

  n = 0;
  after_contains = false;

  while (*line != '\n') {
    c = *line;
    if (isalpha(c)) {
      word[n++] = c;
    } else if (c == ' ' || c == ')') {
      if (word[0] != 0) {
        word[n] = 0;
        if (strcmp(word, "contains")) {
          if (after_contains) {
            add_list(&food->allergens, word);
          } else {
            add_list(&food->ingredients, word);
          }
        }
        word[n = 0] = 0;
      }
    } else if (c == '(') {
      after_contains = true;
    }
    line++;
  }

  if (word[0] != 0)
    add_list(&food->ingredients, word);
  return food;
}

static list_t *extract_allergens(food_t *food)
{
  list_t *allergens, *l;

  allergens = NULL;

  while (food != NULL) {
    l = food->allergens;
    while (l != NULL) {
      if (!contains(allergens, l->string))
        add_list(&allergens, l->string);
      l = l->next;
    }
    food = food->next;
  }
  return allergens;
}

static list_t *find_allergens(food_t *food)
{
  list_t *allergens, *a, *b, *l, *i;
  food_t *f;
  bool done;
  
  allergens = extract_allergens(food);
  a = allergens;

  while (a != NULL) {
    f = food;
    while (f != NULL) {
      if (contains(f->allergens, a->string)) {
        if (a->ingredients == NULL) {
          l = f->ingredients;
          while (l != NULL) {
            add_list(&a->ingredients, l->string);
            l = l->next;
          }
        } else {
          l = a->ingredients;
          while (l != NULL) {
            i = l;
            l = l->next;
            if (!contains(f->ingredients, i->string))
              remove_list(&a->ingredients, i->string);
          }
        }
      }
      f = f->next;
    }
    a = a->next;
  }

  done = false;

  while (!done) {
    done = true;
    a = allergens;
    while (a != NULL) {
      if (len(a->ingredients) == 1) {
        b = allergens;
        while (b != NULL) {
          if (a != b && contains(b->ingredients, a->ingredients->string)) {
            done = false;
            remove_list(&b->ingredients, a->ingredients->string);
          }
          b = b->next;
        }
      }
      a = a->next;
    }
  }
  return allergens;
}

static unsigned ok_ingredients(food_t *food, list_t* allergens)
{
  unsigned n;
  list_t *a, *l;
  bool ok;

  n = 0;

  while (food != NULL) {
    l = food->ingredients;
    while (l != NULL) {
      ok = true;
      a = allergens;
      while (a != NULL) {
        if (contains(a->ingredients, l->string))
          ok = false;
        a = a->next;
      }
      if (ok)
        n++;
      l = l->next;
    }
    food = food->next;
  }

  return n;
}

static int cmp(const void *a, const void *b)
{
  list_t *const *l1 = a;
  list_t *const *l2 = b;
  return strcmp((*l1)->string, (*l2)->string);
}

static void print_canonical_dangerous_ingredient_list(list_t *allergens)
{
  size_t n = len(allergens);
  list_t *dangerous_ingredients[len(allergens)];
  
  for (size_t i = 0; i < n; i++) {
    dangerous_ingredients[i] = allergens;
    allergens = allergens->next;
  }
  qsort(dangerous_ingredients, n, sizeof(char*), cmp);

  printf("Part 2 answer = ");
  for (size_t i = 0; i < n; i++) {
    printf("%s", dangerous_ingredients[i]->ingredients->string);
    if (i < n - 1)
      printf(",");
  }
  printf("\n");
}

static void free_list(list_t *list)
{
  list_t *l = list;
  while (l != NULL) {
    l = l->next;
    free(list->string);
    free_list(list->ingredients);
    free(list);
    list = l;
  }
}

static void free_food(food_t *food)
{
  food_t *f = food;
  while (f != NULL) {
    f = f->next;
    free_list(food->ingredients);
    free_list(food->allergens);
    free(food);
    food = f;
  }
}

int main(void)
{
  food_t *foods, *f;
  list_t *allergens;
  char line[BUFSIZ];

  foods = NULL;

  while (fgets(line, BUFSIZ, stdin) != NULL) {
    f = parse_food(line);
    f->next = foods;
    foods = f;
  }
  allergens = find_allergens(foods);
  
  printf("Part 1 answer = %u\n", ok_ingredients(foods, allergens));
  print_canonical_dangerous_ingredient_list(allergens);

  free_food(foods);
  free_list(allergens);
  return 0;
}
