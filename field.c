#include "field.h"
#include "cursor.h"

int field[FIELD_MAX_Y][FIELD_MAX_X] = {0};

void clear_console()
{
		printf("\e[1;1H");	// top left corner
		printf("\e[0J");	// clear from cursor to the end of terminal
}

void field_init()
{
	for (size_t y=0; y<FIELD_MAX_Y; ++y) {
		for (size_t x=0; x<FIELD_MAX_X; ++x) {
			field[y][x] = EMPTY; // all squares are invisible by default
		}
	}
	field_init_border();
}

void field_init_border()
{
	for (size_t i=0; i<FIELD_MAX_X; ++i) {
		// top horizontal border
		field[0][i] = BORDER;
		// bottom horizontal border
		field[FIELD_MAX_Y-1][i] = BORDER;
	}
	for (size_t i=0; i<FIELD_MAX_Y; ++i) {
		// left vertical border
		field[i][0] = BORDER;
		// right vertical border
		field[i][FIELD_MAX_X-1] = BORDER;
	}
}

void field_render()
{
	//clear_console();
	for (size_t y=0; y<FIELD_MAX_Y; ++y) {
		int col=30;	// start x point of the field
		// for some reason y must begin from 1 or more or you can't see top horizontal border
		int row=y+4;	// start y point of the field
		//int row=y+10;	// start y point of the field
		for (size_t x=0; x<FIELD_MAX_X; ++x) {
			cursor_move(row, col);
			switch (field[y][x]) {
				case BORDER:
					printf("%s", "+ ");
					break;
				case SNAKE:
					printf("%s", "# ");
					break;
				case EMPTY:
					printf("%s", "  ");
					break;
				case FOOD:
					printf("%s", "* ");
					break;
				default:
					printf("%s", "no_such_field_square_code");
					break;

			}
			col+=2; // add +2 for column (x axis) for align it with rows (y axis)
			/*#include <unistd.h>
			sleep(0.05);
			*/
		}
		printf("\n");
	}
}
