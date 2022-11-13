NAME = njvm
REF = refnjvm
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -pedantic -O2

# path to compiler/assambler and tests
NJ = nj/
# Ninja compiler
NJC = $(NJ)njc
# Ninja assabmler
NJA = $(NJ)nja
TESTS = $(NJ)tests/

SRCS = $(wildcard **/*.c) $(wildcard **/**/*.c)
OBJS = $(SRCS:%.c=%.o)

TESTSNJ = $(wildcard $(TESTS)*.nj)
TESTSBIN = $(TESTSNJ:%.nj=%.bin)

run: compile
	./$(NAME) $(args)

compile: $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS)
	rm $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clear:
	rm $(OBJS)

compilenj:
	$(NJC) $(TESTS)$(args) > $(TESTS)tmp.asm
	$(NJA) $(TESTS)tmp.asm $(TESTS)tmp.bin

nj: compile compilenj
	./$(NAME) $(TESTS)tmp.bin

tmp:
	./$(NAME) $(args) $(TESTS)tmp.bin

valgrind: compile compilenj
	valgrind -s ./$(NAME) $(TESTS)tmp.bin

tests: compile $(TESTSBIN)
	rm $(TESTSBIN)
	rm output1.txt output2.txt

%.bin: %.nj
	$(NJC) $< > $(TESTS)tmp.asm
	$(NJA) $(TESTS)tmp.asm $@
	./$(REF) $(args) $@ > output1.txt
	./$(NAME) $(args) $@ > output2.txt
	diff output1.txt output2.txt
