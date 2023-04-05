CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: fork square myshell unbuf_reader unbuf_writer \
 buf_reader buf_writer random fifo_reader fifo_writer

fork: fork.c
	$(CC) $(CFLAGS) -o $@ $<
square: square.c
	$(CC) $(CFLAGS) -o $@ $<
myshell: myshell.c
	$(CC) $(CFLAGS) -o $@ $<
unbuf_reader: unbuf_reader.c
	$(CC) $(CFLAGS) -o $@ $<
unbuf_writer: unbuf_writer.c
	$(CC) $(CFLAGS) -o $@ $<
buf_reader: buf_reader.c
	$(CC) $(CFLAGS) -o $@ $<
buf_writer: buf_writer.c
	$(CC) $(CFLAGS) -o $@ $<
random: random.c
	$(CC) $(CFLAGS) -o $@ $<
pipeshell: pipeshell.c
	$(CC) $(CFLAGS) -o $@ $<
fifo_reader: fifo_reader.c
	$(CC) $(CFLAGS) -o $@ $<
fifo_writer: fifo_writer.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f fork square myshell unbuf_reader unbuf_writer buf_reader buf_writer \
		random pipeshell fifo_reader fifo_writer