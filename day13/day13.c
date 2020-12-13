#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define N 10

typedef struct {
  unsigned delay;
  unsigned bus_id;
} bus_t;

static unsigned wait_time(unsigned earliest, unsigned bus_id)
{
  return (unsigned) ceil(earliest / (double) bus_id) * bus_id - earliest;
}

static unsigned long long least_common_multiple(unsigned long long a, unsigned long long b)
{
  unsigned long long lcm = a;
  while (lcm % b != 0)
    lcm += a;
  return lcm;
}

static bool valid_timestamp(unsigned long long timestamp, bus_t busses[], size_t n)
{
  for (size_t i = 0; i < n; i++)
    if ((timestamp + busses[i].delay) % busses[i].bus_id != 0)
      return false;
  return true;
}

static long long find_timestamp(bus_t busses[], size_t n)
{
  unsigned long long timestamp, lcm;

  timestamp = lcm = busses[0].bus_id;
  for (size_t i = 1; i < n; i++) {
    while (!valid_timestamp(timestamp, busses, i + 1))
      timestamp += lcm;
    lcm = least_common_multiple(lcm, busses[i].bus_id);
  }
  return timestamp;
}

int main(void)
{
  unsigned earliest, bus_id, best_bus_id, delay;
  int c;
  bus_t busses[N];
  size_t n;

  bus_id = best_bus_id = delay = 0;
  n = 0;
  scanf("%u\n", &earliest);
  while ((c = getchar()) != EOF) {
    if (isdigit(c)) {
      bus_id = bus_id * 10 + c - '0';
    } else if (c == ',' || c == '\n') {
      if (bus_id != 0) {
        busses[n].delay = delay;
        busses[n].bus_id = bus_id;
        n++;
        if (bus_id != 0 && (best_bus_id == 0 || wait_time(earliest, best_bus_id) > wait_time(earliest, bus_id)))
          best_bus_id = bus_id;
        bus_id = 0;
      }
      delay++;
    }
  }

  printf("Part 1 answer = %u\n", wait_time(earliest, best_bus_id) * best_bus_id);
  printf("Part 2 answer = %llu\n", find_timestamp(busses, n));
  return 0;
}
