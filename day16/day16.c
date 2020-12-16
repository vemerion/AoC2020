#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define BOUNDS 4

typedef enum { FIELDS, YOUR_TICKET, NEARBY_TICKETS, } INPUT_PHASE;

typedef struct field_t field_t;

struct field_t {
  field_t *next;
  char name[50];
  unsigned bounds[BOUNDS];
  size_t position;
};

static field_t *create_field(char name[], unsigned bounds[])
{
  field_t *field = calloc(1, sizeof(field_t));
  strcpy(field->name, name);
  for (size_t i = 0; i < BOUNDS; i++)
    field->bounds[i] = bounds[i];
  return field;
}


static void add_field(field_t **fields, char name[], unsigned bounds[])
{
  field_t *field;
  if (*fields == NULL)
    *fields = create_field(name, bounds);
  else {
    field = create_field(name, bounds);
    field->next = *fields;
    *fields = field;
  }
}

static void read_ticket(char line[], unsigned *ticket, size_t field_count)
{
  int n;
  for (size_t i = 0; i < field_count; i++) {
    sscanf(line, "%u,%n", ticket + i, &n);
    line += n;
  }
}

static bool valid_field(field_t *fields, unsigned value)
{
  while (fields != NULL) {
    if ((value >= fields->bounds[0] && value <= fields->bounds[1]) || (value >= fields->bounds[2] && value <= fields->bounds[3])) {
      return true;
    }
    fields = fields->next;
  }
  return false;
}

static bool valid_value(field_t *field, unsigned value)
{
  if ((value >= field->bounds[0] && value <= field->bounds[1]) || (value >= field->bounds[2] && value <= field->bounds[3])) {
    return true;
  }
  return false;
}

static bool valid_ticket(field_t *fields, unsigned tickets[], size_t field_count)
{
  for (size_t i = 0; i < field_count; i++)
    if (!valid_field(fields, tickets[i]))
      return false;
  return true;
}

// Not my finest piece of code
static void find_field_positions(field_t *fields, unsigned **tickets, size_t field_count, size_t ticket_count)
{
  field_t *f;
  bool valid_pos;
  size_t i, j, pos;
  bool taken[field_count];
  unsigned possible_pos_count, pos_found_count;

  for (i = 0; i < field_count; i++)
    taken[i] = false;

  pos_found_count = 0;
  while (pos_found_count < field_count) {
    f = fields;
    while (f != NULL) {
      possible_pos_count = 0;
      for (i = 0; i < field_count; i++) {
        if (taken[i])
          continue;
        valid_pos = true;
        for (j = 0; j < ticket_count; j++) {
          if (!valid_ticket(fields, tickets[j], field_count))
            continue;
          if (!valid_value(f, tickets[j][i])) {
            valid_pos = false;
            break;
          }
        }
        if (valid_pos) {
          possible_pos_count++;
          pos = i;
        }
      }
      if (possible_pos_count == 1) {
        taken[pos] = true;
        f->position = pos;
        pos_found_count++;
      }
      f = f->next;
    }
  }
}

static unsigned long long departure_prod(field_t *fields, unsigned *your_ticket)
{
  unsigned long long prod = 1;
  while (fields != NULL) {
    if (!strncmp("departure", fields->name, 9)) {
      prod *= your_ticket[fields->position];
    }
    fields = fields->next;
  }
  return prod;
}

static unsigned ticket_scanning_error_rate(field_t *fields, unsigned **tickets, size_t field_count, size_t ticket_count)
{
  unsigned sum = 0;
  for (size_t i = 0; i < ticket_count; i++)
    for (size_t j = 0; j < field_count; j++)
      if (!valid_field(fields, tickets[i][j]))
        sum += tickets[i][j];
  return sum;
}

static void free_fields(field_t *fields)
{
  field_t *f = fields;
  while (fields != NULL) {
    f = f->next;
    free(fields);
    fields = f;
  }
}

int main(void)
{
  INPUT_PHASE phase;
  field_t *fields = NULL;
  char line[BUFSIZ], name[50];
  unsigned bounds[BOUNDS], *your_ticket, **nearby_tickets;
  size_t field_count, nearby_count;

  phase = FIELDS;
  field_count = nearby_count = 0;
  nearby_tickets = NULL;
  while (fgets(line, BUFSIZ, stdin) != NULL) {
    if (!strcmp(line, "your ticket:\n"))
      phase = YOUR_TICKET;
    else if (!strcmp(line, "nearby tickets:\n"))
      phase = NEARBY_TICKETS;
    else if (strcmp(line, "\n")) {
      switch (phase) {
        case FIELDS:
          sscanf(line, "%[^:]: %u-%u or %u-%u\n", name, bounds, bounds + 1, bounds + 2, bounds + 3);
          add_field(&fields, name, bounds);
          field_count++;
          break;
        case YOUR_TICKET:
          your_ticket = malloc(field_count * sizeof(unsigned));
          read_ticket(line, your_ticket, field_count);
          break;
        case NEARBY_TICKETS:
          nearby_tickets = realloc(nearby_tickets, ++nearby_count * sizeof(unsigned*));
          nearby_tickets[nearby_count - 1] = malloc(field_count * sizeof(unsigned));
          read_ticket(line, nearby_tickets[nearby_count - 1], field_count);
          break;
        default:
          break;
      }
    }
  }
  printf("Part 1 answer = %u\n", ticket_scanning_error_rate(fields, nearby_tickets, field_count, nearby_count));
  find_field_positions(fields, nearby_tickets, field_count, nearby_count);
  printf("Part 2 answer = %llu\n", departure_prod(fields, your_ticket));

  free(your_ticket);
  for (size_t i = 0; i < nearby_count; i++)
    free(nearby_tickets[i]);
  free(nearby_tickets);
  free_fields(fields);
  return 0;
}
