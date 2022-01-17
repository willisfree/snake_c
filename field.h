#ifndef __FIELD__H
#define __FIELD__H

#include <stdio.h>

#define FIELD_MAX_X 20
#define FIELD_MAX_Y FIELD_MAX_X 

extern int field[FIELD_MAX_Y][FIELD_MAX_X];

enum elements {
	BORDER=0, SNAKE, EMPTY, FOOD
};


void field_render();
void field_init();
void field_init_border();

#endif
