#include "ant_engine.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t
palette_size(colour_t *array)
{
	size_t i = 0;
	while (array[i] != NOTACOLOR) {
		i++;
	}
	return i;
}

#ifndef ASSEMBLY
void *
paint(void *ant_p, void *grid_p, void *palette_p, void *rules_p, uint32_t iterations)
{
	ant_t *ant = ant_p;
	square_grid_t *grid = grid_p;
	rotation_t *rules = rules_p;
	colour_t *palette = palette_p;

	orientation_t rotaciones[4][2];
	rotaciones[NORTH][LEFT] = WEST;
	rotaciones[NORTH][RIGHT] = EAST;
	rotaciones[SOUTH][LEFT] = EAST;
	rotaciones[SOUTH][RIGHT] = WEST;
	rotaciones[EAST][LEFT] = NORTH;
	rotaciones[EAST][RIGHT] = SOUTH;
	rotaciones[WEST][LEFT] = SOUTH;
	rotaciones[WEST][RIGHT] = NORTH;

	size_t palette_len = palette_size(palette);

	rotation_t colour_rule[MAXCOLOR];
	for (size_t i = 0; i < palette_len; i++) {
		colour_rule[palette[i]] = rules[i];
	}

	for (uint32_t i = 1; i <= iterations; i++) {
		if (i == palette_len) {
			iterations = iterations - i;
			i = 0;
		}
		orientation_t proxima_orientacion =
		        rotaciones[ant->o]
		                  [colour_rule[grid->grid[ant->x][ant->y]]];
		grid->grid[ant->y][ant->x] = palette[i];

		if (proxima_orientacion == NORTH) {
			int32_t new_y = ant->y - 1;
			if (new_y < 0)
				ant->y = grid->height - 1;
			else
				ant->y = new_y;
		} else if (proxima_orientacion == SOUTH) {
			ant->y++;
			if (ant->y == grid->height)
				ant->y = 0;
		} else if (proxima_orientacion == EAST) {
			ant->x++;
			if (ant->x == grid->width)
				ant->x = 0;
		} else {
			int32_t new_x = ant->x - 1;
			if (new_x < 0)
				ant->x = grid->width - 1;
			else
				ant->x = new_x;
		}
		ant->o = proxima_orientacion;
	}
	return grid;
}
#else
void *
paint(void *ant, void *grid, void *palette, void *rules, uint32_t iterations)
{					\
	printf("%u\n",paint_a(ant, grid, palette, rules, iterations));
	return grid;
}
#endif

void *
make_rules(char *spec)
{
	size_t rules_size = (strlen(spec) / 2) + 1 + 1;
	rotation_t *rules = malloc(rules_size * sizeof(rotation_t));

	size_t i = 0, j = 0;
	for (; spec[i] != '\0'; i++) {
		char rule = spec[i];
		switch (rule) {
		case 'R':
			rules[j] = RIGHT;
			break;
		case 'L':
			rules[j] = LEFT;
			break;
		default:
			continue;
		}
		j++;
	}
	rules[rules_size - 1] = NOTAROTATION;
	return rules;
}

void *
make_palette(char *colours)
{
	size_t palette_size = ((strlen(colours) / 2) + 1 + 1);
	colour_t *palette = malloc(palette_size * sizeof(colour_t));

	size_t i = 0, j = 0;
	for (; colours[i] != '\0'; i++) {
		char colour = colours[i];
		switch (colour) {
		case 'R':
			palette[j] = RED;
			break;
		case 'B':
			palette[j] = BLUE;
			break;
		case 'G':
			palette[j] = GREEN;
			break;
		case 'Y':
			palette[j] = YELLOW;
			break;
		case 'W':
			palette[j] = WHITE;
			break;
		case 'N':
			palette[j] = BLACK;
			break;
		default:
			continue;
		}
		j++;
	}
	palette[palette_size - 1] = NOTACOLOR;
	return palette;
}
