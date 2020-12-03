#include <stdio.h>

// Hardcoded for input size
#define ROWS 323
#define COLS 31

static unsigned encountered_trees(char map[ROWS][COLS], size_t drow, size_t dcol)
{
  unsigned trees = 0;
  size_t row, col;
  row = col = 0;

  while (row < ROWS) {
    if (map[row][col] == '#')
      trees++;
    row += drow;
    col = (col + dcol) % COLS;
  }
  return trees;
}

static unsigned long long encountered_trees_several(char map[ROWS][COLS], size_t drows[], size_t dcols[], size_t n)
{
  unsigned long long trees = 1;
  for (size_t i = 0; i < n; i++)
    trees *= encountered_trees(map, drows[i], dcols[i]);
  return trees;
}

int main(void)
{
  int c;
  char map[ROWS][COLS];
  size_t row, col;

  row = col = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      row++;
      col = 0;
    } else {
      map[row][col] = c;
      col++;
    }
  }

  printf("Part 1 answer = %u\n", encountered_trees(map, 1, 3));
  size_t drows[] = { 1, 1, 1, 1, 2 };
  size_t dcols[] = { 1, 3, 5, 7, 1 };
  printf("Part 2 answer = %llu\n", encountered_trees_several(map, drows, dcols, 5));
}
