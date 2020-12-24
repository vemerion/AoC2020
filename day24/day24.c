#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 1024

typedef struct tile_t tile_t;

struct tile_t {
  int x, y, z;
  bool white, updated;
  tile_t *next;
};

static size_t index(int x, int y, int z) {
  return abs((x * 31 + y) * 37 + z) % N;
}

static void flip(tile_t **tiles, int x, int y, int z)
{
  tile_t *t, *new;
  size_t idx;

  idx = index(x, y, z);
  
  if (tiles[idx] == NULL) {
    new = calloc(1, sizeof(tile_t));
    new->x = x;
    new->y = y;
    new->z = z;
    new->white = true;
    tiles[idx] = new;
    return;
  }

  t = tiles[idx];
  while (t->next != NULL && !(t->x == x && t->y == y && t->z == z))
    t = t->next;

  if (t->x == x && t->y == y && t->z == z) {
    t->updated = !t->white;
    return;
  }

  new = calloc(1, sizeof(tile_t));
  new->x = x;
  new->y = y;
  new->z = z;
  new->white = true;
  t->next = new;
}

static unsigned black_count(tile_t **tiles)
{
  tile_t *t;
  unsigned count;

  count = 0;

  for (size_t i = 0; i < N; i++) {
    t = tiles[i];
    while (t != NULL) {
      if (!t->white)
        count++;
      t = t->next;
    }
  }
  return count;
}

static bool isblack(tile_t **tiles, int x, int y, int z)
{
  tile_t *t;
  
  t = tiles[index(x, y, z)];

  if (t == NULL)
    return false;

  while (t != NULL) {
    if (t->x == x && t->y == y && t->z == z)
      return !t->white;
    t = t->next;
  }

  return false;
}

static int adjacent_black(tile_t **tiles, int x, int y, int z)
{
  int count = 0;
  for (int i = -1; i < 2; i++)
    for (int j = -1; j < 2; j++)
      for (int k = -1; k < 2; k++)
        if (i + j + k == 0 && (i != 0 || j != 0 || k != 0) && isblack(tiles, x + i, y + j, z + k))
          count++;
  return count;
}

static void update(tile_t **tiles, int x, int y, int z)
{
  bool black = isblack(tiles, x, y, z);
  int adj_black = adjacent_black(tiles, x, y, z);

  if ((black && (adj_black == 0 || adj_black > 2)) || (!black && adj_black == 2))
    flip(tiles, x, y, z);
}

static void update_tiles(tile_t **tiles)
{
  tile_t *t;
  for (size_t i = 0; i < N; i++) {
    t = tiles[i];
    while (t != NULL) {
      t->white = t->updated;
      t = t->next;
    }
  }
}

static int living_art(tile_t **tiles)
{
  tile_t *t;
  for (size_t day = 0; day < 100; day++) {
    for (size_t n = 0; n < N; n++) {
      t = tiles[n];
      while (t != NULL) {
        for (int i = -1; i < 2; i++)
          for (int j = -1; j < 2; j++)
            for (int k = -1; k < 2; k++)
              if (i + j + k == 0)
                update(tiles, t->x + i, t->y + j, t->z + k);
        t = t->next;
      }
    }
    update_tiles(tiles);
  }

  return black_count(tiles);
}

static void free_tiles(tile_t **tiles)
{
  tile_t *t;
  for (size_t i = 0; i < N; i++) {
    t = tiles[i];
    while (t != NULL) {
      t = t->next;
      free(tiles[i]);
      tiles[i] = t;
    }
  }
}

int main(void)
{
  tile_t *tiles[N] = { 0 };
  int prev, c, x, y, z;
  
  x = y = z = prev = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      flip(tiles, x, y, z);
  update_tiles(tiles);
      x = y = z = 0;
    } else {
      if (c == 's' || c == 'n') {
        prev = c;
      } else {
        if (prev == 's' && c == 'e') {
          z++;
          y--;
        } else if (prev == 's' && c == 'w') {
          x--;
          z++;
        } else if (prev == 'n' && c == 'w') {
          z--;
          y++;
        } else if (prev == 'n' && c == 'e') {
          x++;
          z--;
        } else if (c == 'e') {
          x++;
          y--;
        } else if (c == 'w') {
          x--;
          y++;
        }

        prev = 0;
      }
    }
  }

  printf("Part 1 answer = %u\n", black_count(tiles));
  printf("Part 2 answer = %u\n", living_art(tiles));

  free_tiles(tiles);
  return 0;
}
