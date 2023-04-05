CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: counter

counter: counter.c
	$(CC) $(CFLAGS) -o $@ $<

counter_pass: counter_pass.c
	$(CC) $(CFLAGS) -o $@ $<

counter_three_times: counter_three_times.c
	$(CC) $(CFLAGS) -o $@ $<

counter_print: counter_print.c
	$(CC) $(CFLAGS) -o $@ $<

counter_file: counter_file.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f counter counter_pass counter_three_times \
		  counter_print counter_file