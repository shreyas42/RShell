CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/shell.o

$(OBJDIR)/shell.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<