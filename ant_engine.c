#include "ant_engine.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_COLOR 6

size_t array_size(void* array){
	int *new_array = (int*)array;
	size_t i = 0;
	while (new_array[i]!=-1){
		i++;
	}
	return i;
}

void*
paint(void *ant, void *grid, void *palette, void *rules,  uint32_t iterations)
{
	ant_t *new_ant = ant;
	square_grid_t *new_grid = grid;
	int *new_rules = rules;
	int *new_palette = palette;

	orientation_t estados[4][2];
  	estados[NORTH][LEFT] = WEST;
  	estados[NORTH][RIGHT] = EAST;
  	estados[SOUTH][LEFT] = EAST;
	estados[SOUTH][RIGHT] = WEST;
	estados[EAST][LEFT] = NORTH;
	estados[EAST][RIGHT] = SOUTH;
	estados[WEST][LEFT] = SOUTH;
	estados[WEST][RIGHT] = NORTH;

	int colour_rule[MAX_NUM_COLOR];
	for(uint32_t i = 0; new_palette[i]!=-1; i++){
		colour_rule[new_palette[i]] = new_rules[0];
	}
	 
	for(uint32_t i=1; i<=iterations; i++){
		if(i == array_size(palette)){
			iterations = iterations -i;
			i = 0;
		}
		orientation_t proximo_estado=estados[new_ant->o][colour_rule[new_grid->grid[new_ant->x][new_ant->y]]];
		new_grid->grid[new_ant->y][new_ant->x] = new_palette[i];

		if(proximo_estado==NORTH){
			new_ant->y--;
			if(new_ant->y < 0) new_ant->y = new_grid->height-1;
		} else if(proximo_estado==SOUTH){
			new_ant->y++;
			if(new_ant->y == new_grid->height) new_ant->y = 0;
		} else if(proximo_estado==EAST){
			new_ant->x++;
			if(new_ant->x == new_grid->width) new_ant->x = 0;
		} else{
			new_ant->x--;
			if(new_ant->x < 0) new_ant->x = new_grid->width-1;
		}
		new_ant->o=proximo_estado;
	}
  return grid;
}

void*
make_rules(char *spec)
{
  	size_t rules_size = (strlen(spec)/2)+1+1;
	rotation_t *rules = malloc(rules_size*sizeof(rotation_t));
	
	for(uint32_t i=0; spec[i]!='\0'; i++){
		char rule = spec[i];
		switch(rule) {
    	case 'R': 
     		rules[i/2] = RIGHT;
        break;
     	case 'L': 
     		rules[i/2] = LEFT;
        break;      
		}
	}
	rules[rules_size-1] = -1;
  	return rules;
}

void*
make_palette(char *colours)
{
	size_t palette_size = ((strlen(colours)/2)+1+1);
	colour_t *palette = malloc(palette_size*sizeof(colour_t));
	
	for(size_t i=0; colours[i]!='\0'; i++){
		char colour = colours[i];
		switch(colour) {
    	case 'R': 
     		palette[i/2] = RED;
        break;
     	case 'B': 
     		palette[i/2] = BLUE;
        break;
      	case 'G': 
      	palette[i/2] = GREEN;
        break;
      	case 'Y': 
      	palette[i/2] = YELLOW;
        break;
      case 'W': 
      	palette[i/2] = WHITE;
        break;
      case 'N': 
      	palette[i/2] = BLACK;
        break;
		}
	}
	palette[palette_size-1] = -1;
  return palette;
}

