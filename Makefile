CC=gcc
CFLAGS=`pkg-config --cflags elementary` -Wall -O2
LDFLAGS=`pkg-config --libs elementary`

SRC=$(wildcard src/*.c)

all:
	$(CC) $(SRC) -o nl-ease $(CFLAGS) $(LDFLAGS)

clean:
	rm -f nl-ease
