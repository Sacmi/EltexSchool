CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCDIRS = 4.1 4.2 4.3 4.4

SOURCES = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
EXECUTABLES = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(EXECUTABLES)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES)
