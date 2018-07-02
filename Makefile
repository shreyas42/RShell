CC = gcc
CFLAGS = -g -Wall
OBJDIR = bin


all: rshell

$(OBJDIR)/shell.o:
	$(MAKE) -C Shell/

$(OBJDIR)/parser.o:
	$(MAKE) -C Parser/

$(OBJDIR)/command.o:
	$(MAKE) -C Parser/

$(OBJDIR)/executor.o:
	$(MAKE) -C Executor/

rshell: $(OBJDIR)/shell.o $(OBJDIR)/parser.o $(OBJDIR)/command.o $(OBJDIR)/executor.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) rshell $(OBJDIR)/*.o *~
