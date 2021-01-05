#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define N 2048

typedef struct round_t round_t;

typedef struct list_t list_t;

struct list_t {
  list_t *prev, *next;
  int value;
};

struct round_t {
  round_t *next;
  list_t *p1, *p2;
};


static bool equals(list_t *l1, list_t *l2) {
  list_t *h1, *h2;

  if (l1 == NULL && l2 == NULL)
    return true;
  else if (l1 == NULL || l2 == NULL)
    return false;

  h1 = l1;
  h2 = l2;

  do {
    if (l1->value != l2->value)
      return false;
    l1 = l1->next;
    l2 = l2->next;
  } while (l1 != h1 && l2 != h2);

  return l1 == h1 && l2 == h2;
}

static int score(list_t *player)
{
  list_t *h;
  int s, i;

  if (player == NULL)
    return 0;

  s = 0;
  i = 1;

  player = player->prev;
  h = player;

  do {
    s += player->value * i++;
    player = player->prev;
  } while (player != h);

  return s;
}

static void add_last(list_t **list, int value)
{
  list_t *l, *e, *s;

  l = calloc(1, sizeof(list_t));
  l->value = value;

  if (*list == NULL) {
    l->prev = l;
    l->next = l;
    *list = l;
    return;
  }

  s = *list;
  e = s->prev;
  l->next = s;
  l->prev = e;
  s->prev = l;
  e->next = l;
}

static list_t *copy(list_t *l, int count)
{
  list_t *c, *h;

  if (l == NULL)
    return NULL;

  c = NULL;
  h = l;
  
  do {
    add_last(&c, l->value);
    l = l->next;
    count--;
  } while (l != h && count != 0);
  return c;
}

static void free_list(list_t **list)
{
  list_t *h, *l1, *l2;

  if (*list == NULL)
    return;

  h = l1 = l2 = *list;
  
  do {
    l1 = l1->next;
    free(l2);
    l2 = l1;
  } while (l1 != h);

  *list = NULL;
}

static size_t index(list_t *p1, list_t *p2)
{
  return score(p1) % N;
}

static bool contains(list_t *p1, list_t *p2, round_t **rounds)
{
  round_t *round = rounds[index(p1, p2)];

  while (round != NULL) {
    if (equals(round->p1, p1) && equals(round->p2, p2))
      return true;
    round = round->next;
  }
  return false;
}

static void add_round(list_t *p1, list_t *p2, round_t **rounds)
{
  size_t idx;
  round_t *r, *new;

  idx = index(p1, p2);
  new = calloc(1, sizeof(round_t));
  new->p1 = copy(p1, -1);
  new->p2 = copy(p2, -1);

  if (rounds[idx] == NULL) {
    rounds[idx] = new;
    return;
  }
  
  r = rounds[idx];
  while (r->next != NULL)
    r = r->next;

  r->next = new;
}

static void free_rounds(round_t **rounds) {
  round_t *r, *s;

  for (size_t i = 0; i < N; i++) {
    r = s = rounds[i];
    while (r != NULL) {
      r = r->next;
      free_list(&s->p1);
      free_list(&s->p2);
      free(s);
      s = r;
    }
  }

  free(rounds);
}


static int len(list_t *l)
{
  list_t *h;
  int n;

  if (l == NULL)
    return 0;

  n = 0;
  h = l;
  do {
    n++;
    l = l->next;
  } while (l != h);
  return n;
}


static int remove_first(list_t **list)
{
  list_t *l;
  int value;

  l = *list;
  
  if (*list == NULL)
    return -1;

  if (l->next == l) {
    value = l->value;
    free(l);
    *list = NULL;
  } else {
    value = l->value;
    l->prev->next = l->next;
    l->next->prev = l->prev;
    *list = l->next;
    free(l);
  }

  return value;
}

static void print_list(list_t *list)
{
  list_t *head;

  if (list == NULL)
    return;

  head = list;

  do {
    printf("%d ", list->value);
    list = list->next;
  } while (list != head);
  printf("\n");
}


static int combat(list_t **player1, list_t **player2)
{
  while (*player1 != NULL && *player2 != NULL) {
    if ((*player1)->value > (*player2)->value) {
      add_last(player1, (*player1)->value);
      add_last(player1, (*player2)->value);
    } else {
      add_last(player2, (*player2)->value);
      add_last(player2, (*player1)->value);
    }
    remove_first(player1);
    remove_first(player2);
  }

  return score(*player1 != NULL ? *player1 : *player2);
}

static bool subgame(list_t **player1, list_t **player2)
{
  list_t *p1, *p2;
  round_t **rounds;
  int v1, v2;

  rounds = calloc(N, sizeof(round_t*));
  
  while (*player1 != NULL && *player2 != NULL) {
    if (contains(*player1, *player2, rounds)) {
      free_rounds(rounds);
      return true;
    } else {
      add_round(*player1, *player2, rounds);
      v1 = remove_first(player1);
      v2 = remove_first(player2);
      if (len(*player1) >= v1 && len(*player2) >= v2) {
        p1 = copy(*player1, v1);
        p2 = copy(*player2, v2);
        if (subgame(&p1, &p2)) {
          add_last(player1, v1);
          add_last(player1, v2);
        } else {
          add_last(player2, v2);
          add_last(player2, v1);
        }
        free_list(&p1);
        free_list(&p2);
      } else {
        if (v1 > v2) {
          add_last(player1, v1);
          add_last(player1, v2);
        } else {
          add_last(player2, v2);
          add_last(player2, v1);
        }
      }
    }
  }

  free_rounds(rounds);
  return *player1 != NULL;
}

static int recursive_combat(list_t **player1, list_t **player2)
{
  if (subgame(player1, player2)) {
    return score(*player1);
  } else {
    return score(*player2);
  }
}

int main(void)
{
  list_t *player1, *player2, *p1, *p2;
  char line[BUFSIZ];
  bool parse_second;

  player1 = player2 = NULL;
  parse_second = false;

  while (fgets(line, BUFSIZ, stdin) != NULL) {
    if (isdigit(line[0])) {
      if (parse_second) {
        add_last(&player2, atoi(line));
      } else {
        add_last(&player1, atoi(line));
      }
    } else if (!strcmp("Player 2:\n", line)) {
      parse_second = true;
    }
  }
  p1 = copy(player1, -1);
  p2 = copy(player2, -1);
  printf("Part 1 answer = %d\n", combat(&player1, &player2));
  printf("Part 2 answer = %d\n", recursive_combat(&p1, &p2));
  free_list(&player1);
  free_list(&player2);
  free_list(&p1);
  free_list(&p2);
  return 0;
}
