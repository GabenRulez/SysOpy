CC= gcc
ATRYBUTY= -Wall

all: build clean

build:
	$(CC) $(ATRYBUTY) -o sender sender.c
	$(CC) $(ATRYBUTY) -o catcher catcher.c

clean:
	rm -rf sender
	rm -rf catcher