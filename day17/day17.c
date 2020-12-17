#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 16384

typedef struct cube_t cube_t;

struct cube_t {
  cube_t *next;
  int x, y, z, w;
  bool active, new;
};

static cube_t *create_part2(int x, int y, int z, int w, bool active)
{
  cube_t *c = calloc(1, sizeof(cube_t));
  c->x = x;
  c->y = y;
  c->z = z;
  c->w = w;
  c->new = active;
  return c;
}

static size_t index_part2(int x, int y, int z, int w)
{
  return ((size_t) (((x * 31 + y) * 37 + z) * 41 + w)) % N;
}

static void add_cube_part2(cube_t **grid, int x, int y, int z, int w, bool active)
{
  size_t idx = index_part2(x, y, z, w);
  cube_t *c = grid[idx];
  if (c == NULL) {
    grid[idx] = create_part2(x, y, z, w, active);
    grid[idx]->active = grid[idx]->new;
    return;
  }
  while (c->next != NULL) {
    c = c->next;
  }
  c->next = create_part2(x, y, z, w, active);
  c->next->active = c->next->new;
}

static bool is_active_part2(cube_t **grid, int x, int y, int z, int w)
{
  cube_t *c = grid[index_part2(x, y, z, w)];
  while (c != NULL && (c->x != x || c->y != y || c->z != z || c->w != w))
    c = c->next;
  if (c != NULL)
    return c->active;
  return false;
}

static int active_neighbors_part2(cube_t **grid, int x, int y, int z, int w)
{
  int n = 0;
  for (int i = -1; i < 2; i++)
    for (int j = -1; j < 2; j++)
      for (int k = -1; k < 2; k++)
        for (int l = -1; l < 2; l++)
          if ((i != 0 || j != 0 || k != 0 || l != 0) && is_active_part2(grid, x + i, y + j, z + k, w + l))
            n++;
  return n;
}

static void update_part2(cube_t **grid, int x, int y, int z, int w)
{
  int n = active_neighbors_part2(grid, x, y, z, w);
  size_t idx = index_part2(x, y, z, w);
  cube_t *c = grid[idx];
  if (c == NULL) {
    if (n == 3)
      grid[idx] = create_part2(x, y, z, w, true);
    return;
  }

  while (c->next != NULL) {
    if (c->x == x && c->y == y && c->z == z && c->w == w) {
      if (c->active && !(n == 2 || n == 3))
        c->new = false;
      else if (!c->active && n == 3)
        c->new = true;
      return;
    }
    c = c->next;
  }
  if (c->x == x && c->y == y && c->z == z && c->w == w) {
    if (c->active && !(n == 2 || n == 3))
      c->new = false;
    else if (!c->active && n == 3)
      c->new = true;
    return;
  }

  if (n == 3)
    c->next = create_part2(x, y, z, w, true);
}

static void update_grid_part2(cube_t **grid)
{
  cube_t *c;
  for (size_t a = 0; a < N; a++) {
    c = grid[a];
    while (c != NULL) {
      for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
          for (int k = -1; k < 2; k++)
            for (int l = -1; l < 2; l++)
            update_part2(grid, c->x + i, c->y + j, c->z + k, c->w +l);
      c = c->next;
    }
  }

  for (size_t i = 0; i < N; i++) {
    c = grid[i];
    while (c != NULL) {
      c->active = c->new;
      c = c->next;
    }
  }
}

static cube_t *create(int x, int y, int z, bool active)
{
  cube_t *c = calloc(1, sizeof(cube_t));
  c->x = x;
  c->y = y;
  c->z = z;
  c->new = active;
  return c;
}

static size_t index(int x, int y, int z)
{
  return ((size_t) ((x * 31 + y) * 39 + z)) % N;
}

static void add_cube(cube_t **grid, int x, int y, int z, bool active)
{
  size_t idx = index(x, y, z);
  cube_t *c = grid[idx];
  if (c == NULL) {
    grid[idx] = create(x, y, z, active);
    grid[idx]->active = grid[idx]->new;
    return;
  }
  while (c->next != NULL) {
    c = c->next;
  }
  c->next = create(x, y, z, active);
  c->next->active = c->next->new;
}

static bool is_active(cube_t **grid, int x, int y, int z)
{
  cube_t *c = grid[index(x, y, z)];
  while (c != NULL && (c->x != x || c->y != y || c->z != z))
    c = c->next;
  if (c != NULL)
    return c->active;
  return false;
}

static int active_neighbors(cube_t **grid, int x, int y, int z)
{
  int n = 0;
  for (int i = -1; i < 2; i++)
    for (int j = -1; j < 2; j++)
      for (int k = -1; k < 2; k++)
        if ((i != 0 || j != 0 || k != 0) && is_active(grid, x + i, y + j, z + k))
          n++;
  return n;
}

static void update(cube_t **grid, int x, int y, int z)
{
  int n = active_neighbors(grid, x, y, z);
  size_t idx = index(x, y, z);
  cube_t *c = grid[idx];
  if (c == NULL) {
    if (n == 3)
      grid[idx] = create(x, y, z, true);
    return;
  }

  while (c->next != NULL) {
    if (c->x == x && c->y == y && c->z == z) {
      if (c->active && !(n == 2 || n == 3))
        c->new = false;
      else if (!c->active && n == 3)
        c->new = true;
      return;
    }
    c = c->next;
  }
  if (c->x == x && c->y == y && c->z == z) {
    if (c->active && !(n == 2 || n == 3))
      c->new = false;
    else if (!c->active && n == 3)
      c->new = true;
    return;
  }

  if (n == 3)
    c->next = create(x, y, z, true);
}

static void update_grid(cube_t **grid)
{
  cube_t *c;
  for (size_t a = 0; a < N; a++) {
    c = grid[a];
    while (c != NULL) {
      for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
          for (int k = -1; k < 2; k++)
            update(grid, c->x + i, c->y + j, c->z + k);
      c = c->next;
    }
  }

  for (size_t i = 0; i < N; i++) {
    c = grid[i];
    while (c != NULL) {
      c->active = c->new;
      c = c->next;
    }
  }
}

static unsigned active_cubes(cube_t **grid) {
  cube_t *c;
  unsigned count = 0;
  for (size_t i = 0; i < N; i++) {
    c = grid[i];
    while (c != NULL) {
      if (c->active)
        count++;
      c = c->next;
    }
  }
  return count;
}

static void free_grid(cube_t **grid) {
  cube_t *c;
  for (size_t i = 0; i < N; i++) {
    c = grid[i];
    while (c != NULL) {
      c = c->next;
      free(grid[i]);
      grid[i] = c;
    }
  }
}

int main(void)
{
  cube_t *grid[N] = { 0 }, *grid2[N] = { 0 };
  int c, x, y;

  x = y = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      x = 0;
      y++;
    } else {
      add_cube(grid, x, y, 0, c == '#');
      add_cube_part2(grid2, x, y, 0, 0, c == '#');
      x++;
    }
  }

  for (size_t i = 0; i < 6; i++) {
    update_grid(grid);
    update_grid_part2(grid2);
  }

  printf("Part 1 answer = %u\n", active_cubes(grid));
  printf("Part 2 answer = %u\n", active_cubes(grid2));
  
  free_grid(grid);
  free_grid(grid2);
  return 0;
}
