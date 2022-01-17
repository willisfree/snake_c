#ifndef __CURSOR__H
#define __CURSOR__H

#include <stdio.h>

// move cursor using ascii escape sequences
void cursor_move(int row, int col)
{
	if (row < 0 || col < 0) {
		printf("row and col must be >=0\n"
			"cursor won't be moved\n");
	}
	printf("\e[%d;%dH", row, col);
}

void cursor_clear()
{
	printf(" ");
}

void cursor_putc(char ch)
{
	printf("%c", ch);
}

void cursor_puts(char* cstr)
{
	if (!cstr) {
		printf("%s: pointer to a string is NULL", __func__);
		return;
	}

	printf("%s", cstr);
}

#endif
