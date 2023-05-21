CC = gcc
CFLAGS = -Wall -Werror

all: static dynamic

static: libaddsub.a
	$(CC) $(CFLAGS) -o static_example main.c -L. -laddsub

dynamic: libaddsub.so
	$(CC) $(CFLAGS) -o dynamic_example main.c -L. -laddsub -Wl,-rpath,.

libaddsub.a: add.o sub.o
	ar rcs libaddsub.a add.o sub.o

libaddsub.so: add.o sub.o
	$(CC) $(CFLAGS) -shared -o addsub.so add.o sub.o

add.o: add.c
	$(CC) $(CFLAGS) -c add.c

sub.o: sub.c
	$(CC) $(CFLAGS) -c sub.c

clean:
	rm -f *.o *.a *.so *_example