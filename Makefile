CC = gcc
CFLAGS = -c
LDFLAGS = 

all: main


hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -o $@ $<

segment.o: segment.c segment.h
	$(CC) $(CFLAGS) -o $@ $<

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -o $@ $<

main.o: main.c hash.h segment.h parser.h
	$(CC) $(CFLAGS) -o $@ $<

main: main.o hash.o segment.o parser.o
	$(CC) $(LDFLAGS) -o main hash.o segment.o parser.o

test.o: test.c hash.h segment.h parser.h
	$(CC) $(CFLAGS) -o $@ $<

test: hash.o parser.o test.o
	$(CC) $(LDFLAGS) -o test hash.o parser.o test.o 

clean:
	rm -f *.o
	rm -f main
	rm -f test