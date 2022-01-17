#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

// tcflush
#include <termios.h>
#include <unistd.h>
// time
#include <time.h>
// rand
#include <stdlib.h>

#include "field.h"
#include "food.h"
#include "snake.h"
#include "control.h"

int peek()
{
	int val = getchar();
	ungetc(val, stdin);
	return val;
}

void clean_eof(FILE* stream)
{
	if (!stream) {
		printf("%s: stream can't be null pointer\n", __func__);
		return;
	}
	if (feof(stdin))
		clearerr(stdin);
	else
		printf("%s: stream not in EOF state\n", __func__);
}

void clean_stdin(void)
{
	int c;
	do {
		//printf("BEFORE getchar()\n");
		c = getchar();
		/*
		printf("c %sequal EOF\n", c == EOF ? "" : "not ");
		printf("c == EOF %d\n", c == EOF);
		printf("AFTER getchar()\n");
		printf("\t%d\n", c);
		*/
		//printf("%d ", c);
	} while (c != EOF && c != '\n');
	//printf("\n");
	clean_eof(stdin);
}

void handle_keys()
{
	//set_raw_mode(0);
	if (key_pressed() > 0) {
		int key = getchar();
		//printf("--------------Your KEY: %d\n", key);
		switch (key) {
			case 'k':
				snake_change_direction(UP);
				//printf("UP\n");
			break;
			case 'j':
				snake_change_direction(DOWN);
				//printf("DOWN\n");
			break;
			case 'h':
				snake_change_direction(LEFT);
				//printf("LEFT\n");
			break;
			case 'l':
				snake_change_direction(RIGHT);
				//printf("RIGHT\n");
			break;
		}
		//tcflush(0, TCIFLUSH);	// not sure: is it useful when VMIN and VTIME are both 0 (polling read)
		clean_stdin();
	}
	//else
		//printf("--------------NOTHING PRESSED\n");
}
/*
void run()
{
	for (;;) {
		field_render();
		snake_unrender();
		handle_keys();
		snake_move_link_forward(0);
		snake_render();
		snake_check_collision();
		sleep(1);
		//usleep(50000);
	}
}
*/

void run()
{
	for (;;) {
		field_render();
		handle_keys();
		//sleep(1);
		//usleep(500000);
		//usleep(90000);
		//usleep(60000);
		//usleep(60000);
		//usleep(20000);
		/*
		if (snake_get_size() > 20)
			usleep(60000);
		else
			usleep(2000);
			*/
		//snake_run();
		snake_unrender();
		// TODO: add opportunitie to move when near the corner (you can see wrong behaviour on 1s interval)
		snake_move_forward();
		snake_food_check();
		snake_render();
		snake_check_collision();
			//snake_unrender();
			//snake_render();
		food_spawn();

		(rand()%100 > 50) ? snake_change_direction(rand()%4) : false;
		//(rand()%10 == 5) ? snake_change_direction(rand()%4) : false;
	}
}

void restore_term()
{
	restore_prev_mode(0);
}

int main()
{
	srand(time(NULL));
	field_init();
	snake_init(1, 1, 1);
	snake_render();
	set_raw_mode(0);
	atexit(&restore_term);
	run();
}
