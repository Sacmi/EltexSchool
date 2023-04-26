CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -lrt

PROGS = p_fifo_reader p_fifo_writer p_random p_random_multi \
	sv_fifo_reader sv_fifo_writer sv_random sv_random_multi \
	sv_shared sv_shared_i \
	p_shared p_shared_i

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGS)