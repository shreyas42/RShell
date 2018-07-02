CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/executor.o

$(OBJDIR)/executor.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<
