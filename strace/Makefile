CC = gcc
CFLAGS = -Wall -Wextra

all: strace_0 strace_1 strace_2

strace_0: strace_0.c
	$(CC) $(CFLAGS) -o $@ $<

strace_1: strace_1.c
	$(CC) $(CFLAGS) -o $@ $<

strace_2: strace_2.c
	$(CC) $(CFLAGS) -o $@ $<

run_0: strace_0
	./strace_0 $(ARGS)

run_1: strace_1
	./strace_1 $(ARGS)

run_2: strace_2
	./strace_2 $(ARGS)

clean:
	rm -f strace_0 strace_1 strace_2

.PHONY: all clean run_0 run_1 run_2
