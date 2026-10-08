#include <ncursesw/curses.h>
#include <string.h>   // strcmp()
#include <time.h>     // time()
#include <stdlib.h> //for atoi()

//types for stages and events
typedef enum {ARMED, WARNING, ALARM, DISARMED} State;
typedef enum {PIN_NONE, PIN_CORRECT,PIN_WRONG,REARM_KEY,PIN_TIMEOUT} Event;

//defining constants I need
#define PIN "2244"
#define CODE_LENGTH 4
#define TIMEOUT_SECONDS 15

//setting up ncurses
void init_ncurses () {
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);      //makes getch() non-blocking
	keypad(stdscr, TRUE);
	set_escdelay(0);
}




//Display clearing, holding digits typed, seconds left in warning to alarm stage transition
void draw_screen(State state, const char *buffer, int seconds_left) {
erase(); //clears the screen

switch(state) {
case ARMED:
printw("SYSTEM ARMED | ENTER CODE:%s\n", buffer);
break;

case WARNING:
printw("Incorrect code - 1 attempt left\n");
printw("Time Remaining: %d s\n", seconds_left);
printw("Enter code: %s\n", buffer);
break;

case ALARM:
attron(COLOR_PAIR(1));
printw("SIREN ON | MISSILES LOCKED\n");
attroff(COLOR_PAIR(1));
printw("Enter code to disarm: %s", buffer);
break;

case DISARMED:
printw("SYSTEM DISARMED | PRESS A TO REARM OR Q to QUIT");
break;
}
refresh ();
}
//help statement text
void print_help() {
	printf("Home Security Alarm - 4 digit PIN\n");
	printf("Options: -h, --help\n");
	printf("-t SECONDS    Set the retry countdown (default %d)\n", TIMEOUT_SECONDS);
	printf("Controls:\n");
	printf("0-9 to enter 4-digit code\n");
	printf("A to re-arm the system\n");
	printf("Q to quit\n");
	
}
//main
int main(int argc, char *argv[]) {

int timeout_seconds =TIMEOUT_SECONDS;
for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        print_help();
        return 0;
    } else if (strcmp(argv[i], "-t")==0) {
    	if (i+1>=argc) {
    		printf("Error:-t requires a number of seconds\n");
    		return 1;
    	}
    	timeout_seconds = atoi(argv[i+1]);
    	i++;
    	if (timeout_seconds<1) {
    	    	printf("Error: Invalid negative number"); 
    }
}
}

init_ncurses();
start_color();
use_default_colors();
init_pair(1, COLOR_RED, -1);

State state = ARMED;
char buffer[CODE_LENGTH + 1] = "";
int count = 0;
int redraw = 1;
time_t warning_start = 0; //when warning was entered
int last_seconds_left = -1; //last countdown number drawn

while (1) {
	Event event = PIN_NONE;
	int key=getch();
	time_t now = time(NULL);

	if (key != ERR) {
		if (key =='q' || key =='Q') {
			break;
		}
	}

	if (key != ERR) {
			if (key =='a' || key =='A') {
				event=REARM_KEY;
			}
		}

	if (key >='0' && key <='9'){
		buffer[count] = key;
		count ++;
		buffer[count] = '\0';
		redraw = 1;

		if (count == CODE_LENGTH) {
			if (strcmp(buffer, PIN)==0)
			event=PIN_CORRECT;
		
		else {
			event=PIN_WRONG;
		}
		buffer[0]='\0';
		count = 0;
		}
	}
	if (state==WARNING && event==PIN_NONE && now-warning_start>=timeout_seconds) {
		event=PIN_TIMEOUT;
	}
	//next-state logic
	State next_state = state;
	switch (state) {
		case ARMED:
		if (event ==PIN_CORRECT) {
			next_state = DISARMED;
		} else if (event ==PIN_WRONG) {
			next_state = WARNING;
		}
		break;

		case WARNING:
		if (event==PIN_CORRECT) {
			next_state = DISARMED;
		} else if (event ==PIN_WRONG || event == PIN_TIMEOUT) {
			next_state = ALARM;
		}
		break;

		case ALARM:
		if (event==PIN_CORRECT) {
			next_state = DISARMED;
		} else if (event==PIN_WRONG) {
			next_state=ALARM;
		}
		break;

		case DISARMED:
		if (event==REARM_KEY) {
			next_state = ARMED;
		} 
		break;
	}
	if (next_state != state) {
		state=next_state;
		buffer[0]='\0';
		count = 0;
		redraw =1;
		if (state==WARNING) {
			warning_start = now;
		}
	}

	int seconds_left = 0;
	if(state==WARNING) {
		seconds_left = timeout_seconds - (now-warning_start);
		if (seconds_left != last_seconds_left) {
			redraw = 1;
			last_seconds_left = seconds_left;
		}
	}
	
	if (redraw) {
		draw_screen(state, buffer,seconds_left);
		redraw = 0;
	}
}
endwin();
return 0;
}
