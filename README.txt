HOME SECURITY ALARM
A Finite State Machine simulation of a keypad alarm panel
 
========================================================================
								OVERVIEW
========================================================================
This program simulates the keypad of a home security alarm system in
the terminal. The system starts armed and asks for a 4-digit PIN. If the
PIN is correct, the system will be disarmed. If it is incorrect once,
the system will go into a WARNING stage where you have 15 seconds (default)
to enter the correct pin. If the correct combination is not entered during this time,
the system will sound the ALARM until you input the correct code. You do not 
need to press ENTER, the system automatically reads each entry as they go.
 
========================================================================
							  REQUIREMENTS
========================================================================
  - Linux (developed and tested on Fedora)
  - gcc and make
  - The ncurses library and headers. On Fedora, install them with:

        				sudo dnf install ncurses-devel

========================================================================
								BUILDING
========================================================================
From the directory containing alarm.c and the Makefile:
 
make          Builds the program, producing the executable "alarm"
make clean    Removes the executable
 
========================================================================
								RUNNING
========================================================================
./alarm [options]
 
Command line options:
 
-h, --help    Print a short usage summary and exit.
 
-t SECONDS    Set the length of the retry countdown in seconds.
              Must be a positive whole number (default is 15).
              Example:  ./alarm -t 5
 
========================================================================
				CONTROLS (while the program is running)
========================================================================
Controls can be found by utilizing -h or --help in the command line.
 
The default PIN is 2244.
 
========================================================================
							SYSTEM STATES
========================================================================
 
ARMED (the starting state)
The system is guarding and waiting for the PIN.
correct PIN  ->  DISARMED
wrong PIN    ->  WARNING
 
WARNING
Time Remaining: N s
Enter code:
One wrong code has been entered. You have one more attempt before
the countdown reaches zero.
correct PIN          ->  DISARMED
wrong PIN            ->  ALARM
countdown runs out   ->  ALARM
 
ALARM
Enter code to disarm:
The alarm is going off. It stops only when the correct PIN is
entered. Wrong codes are rejected and the alarm continues.
correct PIN  ->  DISARMED
wrong PIN    ->  stays in ALARM
 
DISARMED
The system is off. Digits are ignored.
A  ->  ARMED
 
========================================================================
								NOTES
========================================================================
  - Countdown accuracy: the timer counts in whole seconds, so the alarm
    goes off up to one second before the full countdown has passed.
    With the default of 15 seconds, it goes off 14 to 15 seconds after
    the wrong code.
  - Quit with Q so the terminal is restored properly. If the terminal
    behaves strangely after the program exits (for example, typed text
    is not shown), type "reset" and press Enter.
  - Command line arguments other than -h, --help and -t are ignored.
  
  - Nothing in the loop pauses the program (no scanf(), getc() or sleep()),
    so it reacts to key presses and to the countdown at the same time.
 
========================================================================
								FILES
========================================================================
alarm.c       Source code
Makefile      Build rules (make, make clean)
README.txt    This file
