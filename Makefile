PROGRAM = cat
SOURCES = src/cat.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(SOURCES)

test: $(PROGRAM)
	tests/cat_tests.sh

clean:
	rm -f $(PROGRAM) *.o system_cat.txt my_cat.txt system_err.txt my_err.txt