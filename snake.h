#ifndef __SNAKE__H
#define __SNAKE__H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum direction {
	UP=0, DOWN, LEFT, RIGHT
};

struct link {
    int x;
    int y;
};

struct snake {
	struct link* head;
	int size;
	enum direction dir;
};

//static struct snake sam;

void snake_init(int, int, int);
void snake_render_link(int);
void snake_unrender_link(int);
void snake_move_link_forward(int);
void snake_move_forward();
void snake_render();
void snake_unrender();
void snake_check_collision();
void snake_run();
bool is_opposite_direction(enum direction lhs, enum direction rhs);
void snake_change_direction(enum direction new_dir);
void snake_check_collision();
bool is_link_equal(struct link* lhs, struct link* rhs);
struct link* snake_get_link(int num);
struct link* snake_get_head();
struct link* snake_get_tail();
void snake_add_tail_link();
enum direction snake_get_link_dir(int num);
void snake_food_check();
enum direction get_opposite_direction(enum direction dir);
void snake_turn_around();
void snake_move_link_back(int i);
void snake_step_back();
int snake_get_size();

#endif
