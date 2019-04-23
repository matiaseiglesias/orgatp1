#include "ant_engine.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
    orientation_t o[4][2];
    o[NORTH][LEFT] = WEST;
    o[NORTH][RIGTH] = EST;
    o[SOUTH][LEFT] = EST;
    o[SOUTH][RIGTH] = WEST;
    o[EAST][LEFT] = NORTH;
    o[EAST][RIGTH] = SOUTH;
    o[WEST][LEFT] = SOUTH;
    o[WEST][RIGTH] = NORTH;
} estados_t;

void*
paint(void *ant, void *grid, void *palette, void *rules,  uint32_t iterations)
{
	ant_t *new_ant = ant;
	square_grid_t *new_grid = grid;
	rotation_t *new_rules = rules;
	colour_t *new_palette = palette;

	orientation_t estados[4][2];
  estados[NORTH][LEFT] = WEST;
  estados[NORTH][RIGTH] = EST;
  estados[SOUTH][LEFT] = EST;
	estados[SOUTH][RIGTH] = WEST;
	estados[EAST][LEFT] = NORTH;
	estados[EAST][RIGTH] = SOUTH;
	estados[WEST][LEFT] = SOUTH;
	estados[WEST][RIGTH] = NORTH;
	 
	for(uint32_t i=0; i<iterations; i++){
		orientation_t proximo_estado=estados[ant->o][new_rules[i]];
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
		new_grid->grid[new_ant->x][new_ant->y] = new_palette[i] 
	}
  panic ("Implement me!");
  return grid;
}

void*
make_rules(char *spec)
{
  printf("cantidad de ordenes: %i \n",(int)(strlen(spec)/2)+1);
	rotation_t *rules = malloc((strlen(spec)/2)+1*sizeof(rotation_t));
	
	for(size_t i=0; spec[i]!='\0'; i++){
		char rule = spec[i];
		printf("Elemento del arreglo %i: %c \n",(int)i,spec[i]);
		switch(rule) {
    	case 'R': 
     		rules[i/2] = RIGHT;
        break;
     	case 'L': 
     		rules[i/2] = LEFT;
        break;      
		}
	}
	for(int i=0; i<(strlen(spec)/2)+1; i++){
		printf("Regla %i: %i \n",i, (int)rules[i]);
	}
  //panic("Implement me!");
  return rules;
}

void*
make_palette(char *colours)
{
	printf("cantidad de colores: %i \n",(int)(strlen(colours)/2)+1);
	colour_t *palette = malloc((strlen(colours)/2)+1*sizeof(colour_t));
	
	for(size_t i=0; colours[i]!='\0'; i++){
		char colour = colours[i];
		printf("Elemento del arreglo %i: %c \n",(int)i,colours[i]);
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
	for(int i=0; i<(strlen(colours)/2)+1; i++){
		printf("color %i: %i \n",i, (int)palette[i]);
	}
  //panic("Implement me!");
  return palette;
}

