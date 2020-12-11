#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static char updated_pos_part2(char **layout, size_t row, size_t col, size_t rows, size_t cols)
{
  unsigned adjacent_occupied;
  int dir_row, dir_col;
  size_t curr_row, curr_col;
  int directions[8][2] = { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1, } };
  adjacent_occupied = 0;

  if (layout[row][col] == '.')
    return '.';

  for (size_t i = 0; i < 8; i++) {
    dir_row = directions[i][0];
    dir_col = directions[i][1];

    curr_row = row;
    curr_col = col;
    while (true) {
      if (!(curr_row == row && curr_col == col)) {
        if (layout[curr_row][curr_col] == '#') {
          adjacent_occupied++;
          break;
        } else if (layout[curr_row][curr_col] == 'L')
          break;
      }
      if ((curr_row == 0 && dir_row < 0) || (curr_row == rows - 1 && dir_row > 0) || (curr_col == 0 && dir_col < 0) || (curr_col == cols - 1 && dir_col > 0))
        break;
      curr_row += dir_row;
      curr_col += dir_col;
    }
  }

  if (layout[row][col] == 'L' && adjacent_occupied == 0)
    return '#';
  else if (layout[row][col] == '#' && adjacent_occupied > 4)
    return 'L';
  return layout[row][col];
}

static char updated_pos_part1(char **layout, size_t row, size_t col, size_t rows, size_t cols)
{
  unsigned adjacent_occupied, start_row, start_col;

  if (layout[row][col] == '.')
    return '.';

  adjacent_occupied = 0;
  if (row == 0)
    start_row = 0;
  else
    start_row = row - 1;
  if (col == 0)
    start_col = 0;
  else
    start_col = col - 1;

  for (size_t i = start_row; i < row + 2; i++)
    for (size_t j = start_col; j < col + 2;j++)
      if (i < rows && j < cols && !(i == row && j == col) && layout[i][j] == '#')
        adjacent_occupied++;

  if (layout[row][col] == 'L' && adjacent_occupied == 0)
    return '#';
  else if (layout[row][col] == '#' && adjacent_occupied > 3)
    return 'L';
  return layout[row][col];
}

static void update(char **dest, char **src, size_t rows, size_t cols, char (*updated_pos)(char **, size_t, size_t, size_t, size_t))
{
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      dest[i][j] = updated_pos(src, i, j, rows, cols);
}

static void copy(char **dest, char **src, size_t rows, size_t cols)
{
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      dest[i][j] = src[i][j];
}

static unsigned count_occupied(char **layout, size_t rows, size_t cols)
{
  unsigned occupied = 0;
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      if (layout[i][j] == '#')
        occupied++;
  return occupied;
}

static bool changed(char **layout1, char **layout2, size_t rows, size_t cols)
{
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      if (layout1[i][j] != layout2[i][j])
        return true;
  return false;
}

static unsigned stable_seats(char **layout, size_t rows, size_t cols, char (*updated_pos)(char **, size_t, size_t, size_t, size_t)) {
  char **updated = malloc(rows * sizeof(char*));
  unsigned occupied;
  
  for (size_t i = 0; i < rows; i++)
    updated[i] = malloc(cols * sizeof(char));

  while (true) {
    update(updated, layout, rows, cols, updated_pos);
    if (!changed(layout, updated, rows, cols))
      break;
    copy(layout, updated, rows, cols);
  }

  occupied = count_occupied(updated, rows, cols);

  for (size_t i = 0; i < rows; i++)
    free(updated[i]);
  free(updated);

  return occupied;
}

int main(void)
{
  char **layout1, **layout2;
  int c;
  size_t rows, cols, col;

  rows = cols = col = 0;
  layout1 = malloc(sizeof(char*));

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      layout1 = realloc(layout1, (++rows + 1) * sizeof(char*));
      layout1[rows] = malloc(cols * sizeof(char));
      col = 0;
    } else {
      if (rows == 0) {
        if (cols++ == 0)
          layout1[rows] = malloc(sizeof(char));
        else
          layout1[rows] = realloc(layout1[rows], cols * sizeof(char));
      }
      layout1[rows][col++] = c;
    }
  }

  layout2 = malloc((rows + 1) * sizeof(char*));
  
  for (size_t i = 0; i < rows + 1; i++)
    layout2[i] = malloc(cols * sizeof(char));
  copy(layout2, layout1, rows, cols);

  printf("Part 1 answer = %u\n", stable_seats(layout1, rows, cols, updated_pos_part1));
  printf("Part 2 answer = %u\n", stable_seats(layout2, rows, cols, updated_pos_part2));


  for (size_t i = 0; i < rows + 1; i++) {
    free(layout1[i]);
    free(layout2[i]);
  }
  free(layout1);
  free(layout2);
  return 0;
}
