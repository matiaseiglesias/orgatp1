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

void
rotate(orientation_t *orientation, rotation_t rotation)
{
	if (rotation == LEFT) {
		uint32_t old_x = orientation->x;
		orientation->x = -orientation->y;
		orientation->y = old_x;
	} else if (rotation == RIGHT) {
		uint32_t old_x = orientation->x;
		orientation->x = orientation->y;
		orientation->y = -old_x;
	}
}

void
advance(ant_t *ant, square_grid_t *grid)
{
	if (ant->o.x) {
		int32_t new_x = ant->x + ant->o.x;
		if (new_x < 0)
			ant->x = grid->width - 1;
		else if (new_x == grid->width)
			ant->x = 0;
		else
			ant->x = new_x;
	} else if (ant->o.y) {
		int32_t new_y = ant->y + ant->o.y;
		if (new_y < 0)
			ant->y = grid->height - 1;
		else if (new_y == grid->height)
			ant->y = 0;
		else
			ant->y = new_y;
	}
}

void *
paint(void *ant_p, void *grid_p, void *palette_p, void *rules_p, uint32_t iterations)
{
	ant_t *ant = ant_p;
	square_grid_t *grid = grid_p;
	rotation_t *rules = rules_p;
	colour_t *palette = palette_p;

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
		rotate(&(ant->o), colour_rule[grid->grid[ant->x][ant->y]]);
		grid->grid[ant->y][ant->x] = palette[i];

		advance(ant, grid);
	}
	return grid;
}

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
