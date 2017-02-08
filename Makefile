CC=gcc

CFLAGS=-O2 -pthread -Wall -std=c11
#CFLAGS=-O0 -lpthread -g # para debug

calcula.o: calcula.h calcula.c
	$(CC) $(CFLAGS) calcula.c -c

calculat.o: calcula.h calculat.c
	$(CC) $(CFLAGS) calculat.c -c 

teste1: calcula.o teste1.c 
	$(CC) $(CFLAGS) calcula.o teste1.c -o teste1
	./teste1

teste1t: calculat.o teste1t.c
	$(CC) $(CFLAGS) calculat.o teste1t.c -o teste1t
	./teste1t

all: teste1 teste1t
.PHONY: all teste1 clean
.PHONY: all teste1t clean

