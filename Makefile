SHELL = /bin/sh

CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra -g
LFLAGS = -lcheck -lsubunit -lrt -lpthread -lm
GFLAGS = -fprofile-arcs -ftest-coverage

EXE_NAME = tetris
TEST_NAME = tetris_test
LIB_NAME = tetris.a

LIB_SRC = $(wildcard src/brick_game/tetris/backend/*.c)
TEST_SRC = $(wildcard src/test/*.c)

TEST_O = $(TEST_SRC:.c=.o)
LIB_O = $(LIB_SRC:.c=.o)

GCOV_NAME = gcov_tests.info

all: clean install play
.PHONY: all clean tetris.a install uninstall dvi dist test gcov_report

install: tetris.a
	@$(CC) $(CFLAGS) -c ./src/gui/cli/*.c -L. -l:tetris.a
	@$(CC) $(CFLAGS) src/brick_game/tetris/*.c *.o -o $(EXE_NAME) $(LFLAGS) -L. -l:tetris.a -lncurses
	@mkdir install
	@mv $(EXE_NAME) install/
	@touch install/high_score.txt
	@rm -rf *.o brick_game/tetris/backend/*.o

uninstall:
	@rm -rf install

clean:
	@rm -rf *.o *.a *.gcno *.gcda *.info report tetris_test html tetris.tgz

tetris.a: $(LIB_O)
	@ar rc $(LIB_NAME) $(LIB_O)
	@ranlib $(LIB_NAME)
	@rm -rf src/brick_game/tetris/backend/*.o

play:
	@./install/$(EXE_NAME)

dvi:
	@doxygen src/Doxyfile

dist: uninstall install
	@mkdir Tetris-1.0/
	@cp install/tetris Tetris-1.0/ && cp install/high_score.txt Tetris-1.0/
	tar cvzf tetris.tgz Tetris-1.0/
	@rm -rf Tetris-1.0/

test: $(TEST_O) $(LIB_NAME) install
	@$(CC) $(CFLAGS) $(TEST_O) -o $(TEST_NAME) -L. -l:$(LIB_NAME) $(LFLAGS)
	@rm -rf $(TEST_O)
	@./$(TEST_NAME)
	@rm $(LIB_NAME)

gcov_report: clean
	$(CC) $(CFLAGS) -c $(LIB_SRC) --coverage
	$(CC) $(CFLAGS) -c $(TEST_SRC)
	$(CC) *.o -o $(TEST_NAME) $(LFLAGS) $(GFLAGS)
	./$(TEST_NAME)
	lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d .
	genhtml -o report $(GCOV_NAME)
	@rm -rf *.gcno *.gcda *.gcov $(GCOV_NAME) *.o
