#ifndef __CONTROL__H
#define __CONTROL__H

#include <stdio.h>
#include <sys/select.h>
#include <stdbool.h>

int key_pressed();
bool specified_key_is(int key_num);
int set_raw_mode(int fd);
int restore_prev_mode(int fd);


#endif
