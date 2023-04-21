CC = gcc
CFLAGS = -Wall -Wextra -Werror

PROGS = counter counter_pass counter_three_times \
 counter_print counter_file random sv_reader sv_writer \
 sv_chat sv_server sv_client p_reader p_writer p_chat \
 p_client p_server

all: $(PROGS)

p_reader: p_reader.c
	$(CC) $(CFLAGS) -lrt -o $@ $<

p_writer: p_writer.c
	$(CC) $(CFLAGS) -lrt -o $@ $<

p_chat: p_chat.c
	$(CC) $(CFLAGS) -lrt -o $@ $<

p_client: p_client.c
	$(CC) $(CFLAGS) -lrt -o $@ $<

p_server: p_server.c
	$(CC) $(CFLAGS) -lrt -o $@ $<

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGS)