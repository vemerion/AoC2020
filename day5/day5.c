#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 127
#define MAX_COL 7
#define SEAT_COUNT 127 * 8 + 7

size_t find_your_seat(unsigned seats[])
{
  for (size_t i = 1; i < SEAT_COUNT - 1; i++) {
    if (seats[i - 1] && seats[i + 1] && !seats[i])
      return i;
  }
  return 0;
}

int main(void)
{
  int c;
  unsigned min_row, max_row, min_col, max_col, id, max_id;
  unsigned seats[SEAT_COUNT] = { false }; // Every seat is not taken at the start

  min_row = min_col = max_id = 0;
  max_row = MAX_ROW;
  max_col = MAX_COL;

  while ((c = getchar()) != EOF) {
    if (c == 'F') {
      max_row -= (max_row - min_row) / 2 + 1;
    } else if (c == 'B') {
      min_row += (max_row - min_row) / 2 + 1;
    } else if (c == 'L') {
      max_col -= (max_col - min_col) / 2 + 1;
    } else if (c == 'R') {
      min_col += (max_col - min_col) / 2 + 1;
    } else if (c == '\n') {
      id = max_row * 8 + max_col;
      seats[id] = true;
      if (id > max_id)
        max_id = id;
      min_row = min_col = 0;
      max_row = MAX_ROW;
      max_col = MAX_COL;
    }
  }
  printf("Part 1 answer = %u\n", max_id);
  printf("Part 2 answer = %zu\n", find_your_seat(seats));
}
