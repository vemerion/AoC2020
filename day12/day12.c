#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define DIR_COUNT 4

typedef enum { E = 0, S = 1, W = 2, N = 3 } dir_t;

typedef struct {
  dir_t direction;
  int east_pos;
  int north_pos;
} ship_t;

typedef struct {
  int east_pos;
  int north_pos;
} waypoint_t;

static int manhattan(ship_t *ship)
{
  return abs(ship->east_pos) + abs(ship->north_pos);
}

static void forward(ship_t *ship, int distance)
{
  switch (ship->direction) {
    case E:
      ship->east_pos += distance;
      break;
    case S:
      ship->north_pos -= distance;
      break;
    case W:
      ship->east_pos -= distance;
      break;
    case N:
      ship->north_pos += distance;
      break;
    default:
      break;
  }
}

static void turn(ship_t *ship, int degrees)
{
  ship->direction = (ship->direction + degrees / 90) % DIR_COUNT;
}

static void move_part1(ship_t *ship, char action, int value)
{
  switch (action) {
    case 'N':
      ship->north_pos += value;
      break;
    case 'S':
      ship->north_pos -= value;
      break;
    case 'E':
      ship->east_pos += value;
      break;
    case 'W':
      ship->east_pos -= value;
      break;
    case 'L':
      turn(ship, -value);
      break;
    case 'R':
      turn(ship, value);
      break;
    case 'F':
      forward(ship, value);
      break;
    default:
      break;
  }
}

static void rotate_waypoint(waypoint_t *waypoint, int degrees)
{
  double x, y, length, arg;

  x = waypoint->east_pos;
  y = waypoint->north_pos;
  length = sqrt(x * x + y * y);
  arg = atan2(y, x);
  arg += degrees * acos(-1) / 180.0;
  waypoint->east_pos = round(cos(arg) * length);
  waypoint->north_pos = round(sin(arg) * length);
}

static void move_part2(waypoint_t *waypoint, ship_t *ship, char action, int value)
{
  switch (action) {
    case 'N':
      waypoint->north_pos += value;
      break;
    case 'S':
      waypoint->north_pos -= value;
      break;
    case 'E':
      waypoint->east_pos += value;
      break;
    case 'W':
      waypoint->east_pos -= value;
      break;
    case 'L':
      rotate_waypoint(waypoint, value);
      break;
    case 'R':
      rotate_waypoint(waypoint, -value);
      break;
    case 'F':
      ship->north_pos += waypoint->north_pos * value;
      ship->east_pos += waypoint->east_pos * value;
      break;
    default:
      break;
  }
}

int main(void)
{
  char action;
  int value;
  ship_t *ship1, *ship2;
  waypoint_t *waypoint;

  ship1 = calloc(1, sizeof(ship_t));
  ship2 = calloc(1, sizeof(ship_t));
  waypoint = calloc(1, sizeof(waypoint_t));
  waypoint->east_pos = 10;
  waypoint->north_pos = 1;

  while (scanf("%c%d\n", &action, &value) != EOF) {
    move_part1(ship1, action, value);
    move_part2(waypoint, ship2, action, value);
  }
  printf("Part 1 answer = %d\n", manhattan(ship1));
  printf("Part 2 answer = %d\n", manhattan(ship2));

  free(ship1);
  free(ship2);
  free(waypoint);
  return 0;
}
