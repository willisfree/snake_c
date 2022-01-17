#include "food.h"
#include "field.h"

static bool food_was_eaten = true;

// INFO: there is idea for more original way of spawn food, so it can be spawn under snake, but if you want to implemetn it you need to add second logical layer on field so field will be 2D array of 'struct point { int topl; int bottoml};'. Now food can't be spawn under snale for code simplicity (which was orinial ideas).
void food_spawn()
{
	if (food_was_eaten) {
		int x = 0;
		int y = 0;
		do {
			x = 1 + rand() % (FIELD_MAX_X-2);
			y = 1 + rand() % (FIELD_MAX_Y-2);
		} while (field[y][x] == SNAKE); // food can't be spawn under snake
		field[y][x] = FOOD;
	}
	food_was_eaten = false;
}

void food_eat()
{
	food_was_eaten = true;
}
