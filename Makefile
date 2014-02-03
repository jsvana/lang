CC=clang

PROG=lang
OBJS=main.o atom.o lexer.o

CFLAGS=-ggdb
LIBS=-lreadline

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

run: $(PROG)
	./$(PROG)

clean:
	rm -f $(PROG) $(OBJS)
