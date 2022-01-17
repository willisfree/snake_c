#include <stdlib.h>

#include "snake.h"
#include "field.h"
#include "food.h"

static struct snake sam;

static void update_hide_tail()
{
	// look like this check it's not useful if [sam.head, sam.head+sam.size] memory isn't zero initialized
	if (!(sam.head+sam.size)) {
		printf("snake doesn't have hide tail link\n");
		exit(2);
	}
	sam.head[sam.size] = sam.head[sam.size-1];
}

void snake_init(int x, int y, int sz)
{
	// why less than one?: (example: field begin with border on {0, 0} point, so snake begin from {1, 1})
	if (x<1 || y<1 || x+sz>FIELD_MAX_X-1) {
		printf("snake position invalid\n");
		exit(2);
	}

	sam.head = malloc((sz+1)*sizeof(struct link)); // +1 hide tail link for grow when eat food
	if (!sam.head) {
		printf("snake doesn't want to run\n");
		exit(1);
	}
	sam.size = sz;
	sam.dir = RIGHT;

	// place snake horizontally with head in right side
	for (size_t i=0; i<sam.size; ++i) {
		sam.head[i].x = x+(sam.size-1-i); // initialize snake links so head will be the most right
		sam.head[i].y = y;
	}
	update_hide_tail();
}

void snake_render_link(int link_num)
{
	if (link_num >= sam.size || link_num < 0) {
		printf("snake doesn't have such link: %d\n", link_num);
		exit(3);
	}
	// get coordinates of specified link 
	int x = sam.head[link_num].x;
	int y = sam.head[link_num].y;
/*
		printf("X: %d\n", x);
		printf("Y: %d\n", y);
*/
	// place it on field
	field[y][x] = SNAKE;
}

void snake_unrender_link(int link_num)
{
	if (link_num >= sam.size || link_num < 0) {
		printf("snake doesn't have such link: %d\n", link_num);
		exit(3);
	}
	int x = sam.head[link_num].x;
	int y = sam.head[link_num].y;
	field[y][x] = EMPTY;
}

// check if snake stuck with food
bool snake_eat_food(struct link* head)
{
	// return head position equal food position
	return field[head->y][head->x] == FOOD;
}

void snake_grow()
{
	// add +1 to tail
	struct snake new_sam;
        new_sam.head = malloc((sam.size+1+1)*(sizeof(struct link))); // +1 for new link and +1 for hide tail link
        if (!new_sam.head) {
                printf("malloc failed\n");
                exit(1);
        }
        new_sam.size = sam.size+1;
        new_sam.dir = sam.dir;

        // copy body
        memcpy(new_sam.head, sam.head, (sam.size+1)*sizeof(struct link)); // copy with hide tail link
	//new_sam.head[new_sam.size-1] = new_sam.head[new_sam.size];
        free(sam.head);
        sam = new_sam;

	update_hide_tail();

        // init position of last tail link
	//snake_add_tail_link();
}

void snake_food_check()
{
	if (snake_eat_food(sam.head)) {
		food_eat();
		snake_grow();
	}
}

void snake_render()
{
	for (size_t i=0; i<sam.size; ++i) {
		snake_render_link(i);
	}
}

void snake_unrender()
{
	for (size_t i=0; i<sam.size; ++i) {
		snake_unrender_link(i);
	}
}

enum direction get_opposite_direction(enum direction dir)
{
	if (dir == UP) return DOWN;
	if (dir == DOWN) return UP;
	if (dir == LEFT) return RIGHT;
	if (dir == RIGHT) return LEFT;
}

bool is_opposite_direction(enum direction lhs, enum direction rhs)
{
	if (lhs == UP && rhs == DOWN) return true;
	if (lhs == DOWN && rhs == UP) return true;
	if (lhs == LEFT && rhs == RIGHT) return true;
	if (lhs == RIGHT && rhs == LEFT) return true;
	return false;
}

void snake_change_direction(enum direction new_dir)
{
	if (sam.size > 1 && is_opposite_direction(sam.dir, new_dir)) {
		printf("you can't move in opposite direction");
		return;
	}
	sam.dir = new_dir;
}

void snake_add_tail_link()
{
	struct link* tail = snake_get_tail();
	if (!tail) {
		printf("%s: tail NULL\n", __func__);
		exit(7);
	}

	int pre_tail_i = sam.size-2; // pre_tail index
	struct link* pre_tail = snake_get_link(pre_tail_i);
	if (!pre_tail) {
		printf("%s: pre_tail NULL\n", __func__);
		exit(7);
	}

	switch (snake_get_link_dir(pre_tail_i)) {
		case RIGHT:
			tail->x = pre_tail->x-1;
			tail->y = pre_tail->y;
			break;
		case LEFT:
			tail->x = pre_tail->x+1;
			tail->y = pre_tail->y;
			break;
		case UP:
			tail->y = pre_tail->y+1;
			tail->x = pre_tail->x;
			break;
		case DOWN:
			tail->y = pre_tail->y-1;
			tail->x = pre_tail->x;
			break;
		default:
			printf("unknown direction\n");
			break;
	}
}

void snake_move_head_forward()
{
	struct link* head = snake_get_head();
	switch (sam.dir) {
	case RIGHT:
		head->x++;
		break;
	case LEFT:
		head->x--;
		break;
	case UP:
		head->y--;
		break;
	case DOWN:
		head->y++;
		break;
	default:
		printf("unknown direction\n");
		break;
	}
}

struct link* snake_get_link(int num)
{
	if (sam.size < 1) {
		printf("failed to get snake's link, because snake doesn't even has it.");
		exit(6);
	}
	return sam.head+num;
}

struct link* snake_get_tail()
{
	return snake_get_link(sam.size-1);
}

struct link* snake_get_head()
{
	return snake_get_link(0);
}

// deep copy
static void swap_links(struct link* lhs, struct link* rhs)
{
	struct link tmp = *lhs;
	lhs->x = rhs->x;
	lhs->y = rhs->y;

	rhs->x = tmp.x;
	rhs->y = tmp.y;
}

// make sanke move in opposite direction
void snake_turn_around()
{
	for (int i=0; i<(int)sam.size/2; ++i) {	// explicit narrow cast
		swap_links(snake_get_link(i), snake_get_link(sam.size-1-i));
	}
	sam.dir = get_opposite_direction(sam.dir);
}

void snake_move_forward()
{
	struct snake new_sam;
	new_sam.head = malloc((sam.size+1)*sizeof(struct link)); // +1 for hide tail link
	if (!new_sam.head) {
		printf("malloc failed\n");
		exit(1);
	}
	new_sam.size = sam.size;
	new_sam.dir = sam.dir;

	// copy body
	memcpy(new_sam.head+1, sam.head, (new_sam.size)*sizeof(struct link)); // copy with hide tail link
	snake_move_head_forward();
	// copy head
	new_sam.head[0] = sam.head[0];

	// apply changes
	free(sam.head);
	sam = new_sam;

	//update_hide_tail(); doesn't need here because hide tail link already copied from old sam
}

void snake_move_link_forward(int link_num)
{
	/*
	struct link* head = snake_get_head();

	if (head == sam.head) {
		snake_move_head_forward();
		//return;
	}

	struct snake new_sam;
	new_sam.head = malloc(sam.size*sizeof(struct link));
	if (!new_sam.head) {
		printf("mallov failed\n");
		exit(1);
	}
	new_sam.size = sam.size;
	new_sam.dir = sam.dir;

	// copy head
	new_sam.head[0] = sam.head[0];
	// copy body
	memcpy(new_sam.head+1, sam.head+1, sam.size-1);
	free(sam.head);
	sam = new_sam;

	//printf("WTF\n");
	switch (sam.dir) {
	case RIGHT:
		sam.head[link_num].x++;
		break;
	case LEFT:
		sam.head[link_num].x--;
		break;
	case UP:
		sam.head[link_num].y--;
		break;
	case DOWN:
		sam.head[link_num].y++;
		break;
	default:
		break;
	}
	//printf("after move: %d\n", sam.head[link_num].x);
	//
*/
}

bool is_link_equal(struct link* lhs, struct link* rhs)
{
	return lhs->x == rhs->x
		&& lhs->y == rhs->y;
}

// TODO: maybe add tail regocnition and spawn an error in case we connect our tail
bool snake_check_self_collision()
{
	struct link* head = snake_get_head();
	for (size_t i=1; i<sam.size; ++i) {
		if (is_link_equal(head, sam.head+i))
			return true;
        }
	return false;
}

void snake_step_back()
{
	struct snake new_sam;
       	new_sam.head = malloc((sam.size+1)*sizeof(struct link)); // +1 hide tail link for grow when eat food
	if (!new_sam.head) {
		printf("snake doesn't want to step back\n");
		exit(1);
	}
	new_sam.size = sam.size;
	new_sam.dir = sam.dir;

	// copy with hide tail, which is now part of the snake
	memcpy(new_sam.head, sam.head+1, (sam.size+1)*sizeof(struct link));
	//new_sam.head[new_sam.size-1] = snake_get_link(sam.size);
	// TODO: init new hide tail link
	
	free(sam.head);
	sam = new_sam;
/*
	for (size_t i=0; i<sam.size; ++i) {
		// more expensive way of moving (with compare to snake_move_forward) but more obvious
		snake_move_link_back(old_sam, i);
        }
*/
}

/*
// must be fixed: get firection of link from old snake but update on new
void snake_move_link_back(const struct snake* s, int i)
{
	//enum direction dir = snake_get_link_dir(i);
	switch (snake_get_link_dir(i)) {
		case RIGHT:
			snake_get_link(i)->x--;
			break;
		case LEFT:
			snake_get_link(i)->x++;
			break;
		case UP:
			snake_get_link(i)->y++;
			break;
		case DOWN:
			snake_get_link(i)->y--;
			break;
		default:
			break;
	}
}
*/

void snake_check_collision()
{
	if (sam.head[0].x == FIELD_MAX_X-1 || sam.head[0].y == FIELD_MAX_Y-1
			|| sam.head[0].x == 0 || sam.head[0].y == 0) {
		
		// hack)
		snake_step_back();
		field_init_border();
		snake_change_direction(rand()%4);
		return;
		
		//snake_turn_around();
		//field_init_border();
		return;
		printf("OH, SHIT. This is wall!\n");
		exit(4);
	}
/*
	if (snake_check_self_collision()) {
		printf("OH, SHIT. You stuck in yourself! You shouldn't do this, man!\n");
		exit(4);
	}
	*/
}

void snake_run()
{
	for (size_t i=0; i<sam.size; ++i) {
		snake_unrender_link(i);
		//printf("before move: %d\n", sam.head[0].x);
		snake_move_link_forward(i);
		//printf("after move: %d\n", sam.head[0].x);
		snake_render_link(i);
	}
}

// return num link direction depend on previous link
enum direction snake_get_link_dir(int num)
{
	struct link* current = snake_get_link(num);
        if (!current) {
                printf("%s: current NULL\n", __func__);
                exit(7);
        }
	if (current == sam.head)
		return sam.dir;

	struct link* previous = snake_get_link(num-1);
        if (!previous) {
                printf("%s: previous NULL\n", __func__);
                exit(7);
        }

	// pay attention to origin point of coordiante system when returning direction
	int xdiff = previous->x - current->x;
	int ydiff = previous->y - current->y;
	if (xdiff == -1)	return LEFT;
	else if (xdiff == 1)	return RIGHT;
	else if (ydiff == 1)	return DOWN;
	else if (ydiff == -1)	return UP;
}

int snake_get_size()
{
	return sam.size;
}
