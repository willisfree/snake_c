#include "control.h"

// termios
#include <termios.h>
#include <unistd.h>


// variable for save terminal state between control calls
#define MAX_TTY_SAVE_SLOTS 200
static struct termios saved_tty[MAX_TTY_SAVE_SLOTS] = {0};
static bool saved_tty_list[MAX_TTY_SAVE_SLOTS] = {0};

int key_pressed()
{
	struct timeval tv = {0};	// wait 0 seconds so app will not block for timeout
	int stdinput = 0;
	fd_set rdfs;				// will be modified by select and will contain num of fds which have some data
	FD_ZERO(&rdfs);
	FD_SET(stdinput, &rdfs);

	int triggered_fds = select(1, &rdfs, NULL, NULL, &tv);
	// don't rely on the value of tv now (select may update it)
	if (triggered_fds == -1)
			perror("select() fail");

	return triggered_fds;
}

bool specified_key_is(int key_num)
{
	int key = getchar();
	return key == key_num;
}

int set_raw_mode(int fd)
{
        if (fd < 0) {
                printf("%s: file descriptor must be >= 0\n", __func__);
                return -1;
        }

        struct termios terminal;
        if (tcgetattr(fd, &terminal) < 0) {
                perror("tcgetattr(): ");
                return -1;
        }

	if (fd > MAX_TTY_SAVE_SLOTS-1) {
		printf("previous tty value wasn't saved because fd number more than max array index\n");
	}
	else {
		saved_tty[fd] = terminal;	// save for future restore
		saved_tty_list[fd] = true;
	}

        //cfmakeraw(&terminal);
	terminal.c_lflag &= ~(ICANON | ECHO);
	//terminal.c_iflag &= ~(IGNBRK | BRKINT);
	terminal.c_cc[VMIN] = 0;
	terminal.c_cc[VTIME] = 0;
        if (tcsetattr(fd, TCSANOW, &terminal) < 0) {
                perror("tsetattr(): ");
                return -1;
        }

        return 0;
}

//review
int restore_prev_mode(int fd)
{
        if (fd < 0) {
                printf("%s: file descriptor must be >= 0\n", __func__);
                return -1;
        }

	if (fd > MAX_TTY_SAVE_SLOTS-1) {
		printf("previous tty value wasn't restored because fd number more than max array index\n");
		return -1;
	}
	else if (saved_tty_list[fd]) {
		if (tcsetattr(fd, TCSANOW, &saved_tty[fd]) < 0) {
			perror("tsetattr(): ");
			return -1;
		}
		return 0;
	}
	else {
		printf("previous tty value wasn't restored because previous value for this fd wasn't saved\n");
		return -1;
	}

	return 0;
}
