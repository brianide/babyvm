CC=gcc
CFLAGS=-I.
DEPS = main.h
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

babyvm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

