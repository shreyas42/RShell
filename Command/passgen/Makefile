CC = gcc
CFLAGS = -g -Wall
OBJDIR = ../../sbin

all: $(OBJDIR)/passgen

$(OBJDIR)/passgen: *.c *.h
	$(CC) $(CFLAGS) -o $@ $< 
