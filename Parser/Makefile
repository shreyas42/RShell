CC = gcc
CFLAGS = -g -Wall

all: output

output: y.tab.o lex.yy.o command.o
	$(CC) $(CFLAGS) -o $@ $^ -ll -ly

y.tab.o:
	yacc -d parser.y
	$(CC) -c y.tab.c
lex.yy.o:
	lex parser.l
	$(CC) -c lex.yy.c

test: test.o command.o
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.c util.h
	$(CC) $(CFLAGS) -c $^

command.o: command.c util.h
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) test output *~ *.o lex.yy.c y.tab.h y.tab.c *.gch