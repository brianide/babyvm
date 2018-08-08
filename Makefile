CC=gcc
CFLAGS=-I.

build: main.o
	$(CC) -o babyvm main.o
