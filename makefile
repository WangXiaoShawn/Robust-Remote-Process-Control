CFLAGS=-Wall -Wextra -g
LDLIBS=-lpthread
CC=gcc
BIN=./bin
VPATH=./comm:./SysMng:./Child:./prockill

all: Prockill SysMgr allchild install

SysMgr: SysMgr.o configreader.o

allchild: child
	cp child child1
	cp child child2
	cp child child3
child: child.o
Prockill: Prockill.o configreader.o
test: test.o configreader.o
clean:
		rm -f *.o $(BIN)/Prockill $(BIN)/SysMgr $(BIN)/child*


install:
		mv child* Prockill SysMgr $(BIN)

