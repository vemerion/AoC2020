#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 10
#define SIDES 4

typedef struct {
  unsigned id;
  char sides[SIDES * 2][N + 1];
} tile_t;

static tile_t *parse_tile()
{
  tile_t *tile = NULL;
  size_t i = 0;
  char line[BUFSIZ];

  while (fgets(line, BUFSIZ, stdin) != NULL) {
    if (line[0] == 'T') {
      tile = calloc(1, sizeof(tile_t));
      sscanf(line, "%*s %u:\n", &tile->id);
    } else if (line[0] != '\n') {
      tile->sides[1][i] = line[N - 1];
      tile->sides[3][i] = line[0];
      if (i == 0) {
        strncpy(tile->sides[0], line, N + 1);
        tile->sides[0][N] = '\0';
      } else if (i == N - 1) {
        strncpy(tile->sides[2], line, N + 1);
        tile->sides[2][N] = '\0';
        return tile;
      }
      i++;
    }
  }
  return tile;
}

static void flip_sides(tile_t *tile)
{
  for (size_t i = 0; i < SIDES; i++) {
    for (size_t j = 0; j < N; j++) {
      tile->sides[i + SIDES][j] = tile->sides[i][N - 1 - j];
    }
    tile->sides[i + SIDES][N] = '\0';
  }
}

static bool is_corner(tile_t *tile, tile_t **tiles, size_t n)
{
  unsigned matched_sides = 0;
  
  for (size_t i = 0; i < n; i++)
    if (tile != tiles[i])
      for (size_t j = 0; j < SIDES * 2; j++)
        for (size_t k = 0; k < SIDES * 2; k++)
          if (!strcmp(tile->sides[j], tiles[i]->sides[k]))
            matched_sides++;

  return matched_sides == 4;
}

static unsigned long long corner_id_mul(tile_t **tiles, size_t n)
{
  unsigned long long prod = 1;
  
  for (size_t i = 0; i < n; i++)
    if (is_corner(tiles[i], tiles, n))
      prod *= tiles[i]->id;
  return prod;
}

static void free_tiles(tile_t **tiles, size_t n)
{
  for (size_t i = 0; i < n; i++) {
    free(tiles[i]);
  }
  free(tiles);
}

int main(void)
{
  size_t n;
  tile_t **tiles, *tile;

  n = 0;
  tiles = NULL;
  
  while ((tile = parse_tile()) != NULL) {
    tiles = realloc(tiles, (n + 1) * sizeof(tile_t*));
    flip_sides(tile);
    tiles[n++] = tile;
  }

  printf("Part 1 answer = %llu\n", corner_id_mul(tiles, n));

  free_tiles(tiles, n);
  return 0;
}
