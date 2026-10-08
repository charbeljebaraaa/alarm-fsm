make: alarm.c
	gcc alarm.c -lncurses -o alarm

clean:
	rm alarm
