CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../bin

all: $(OBJDIR)/parser.o $(OBJDIR)/command.o

$(OBJDIR)/command.o: c*.c c*.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/parser.o: p*.c p*.h
	$(CC) $(CFLAGS) -c -o $@ $<

