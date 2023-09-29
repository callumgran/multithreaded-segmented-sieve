# Callum Gran 2023
# See LICENSE for license info

SRC = ./

CFLAGS = -Iinclude -Wall -Wextra -Wshadow -std=c11 -lm -D_POSIX_C_SOURCE=200809L -O3

.PHONY: format clean tags bear $(OBJDIR)
SEGMENTED_SIEVE = segmented_sieve
SEGMENTED_SIEVE_PRINT = segmented_sieve_print
SEGMENTED_SIEVE_ERATOSTHENES = segmented_sieve_eratosthenes

clean:
	rm -rf $(OBJDIR) $(SEGMENTED_SIEVE) $(SEGMENTED_SIEVE_PRINT) $(SEGMENTED_SIEVE_ERATOSTHENES)

tags:
	@ctags -R

bear:
	@bear -- make

format:
	python format.py

segmented_sieve:
	$(CC) ./segmented_sieve.c ./test_program.c -lm $(CFLAGS) -o $(SEGMENTED_SIEVE)

segmented_sieve_print:
	$(CC) ./segmented_sieve.c ./test_program.c -lm $(CFLAGS) -o $(SEGMENTED_SIEVE) -DPRINT_PRIMES

segmented_sieve_eratosthenes:
	$(CC) ./segmented_sieve.c ./test_program.c -lm $(CFLAGS) -o $(SEGMENTED_SIEVE) -DERATOSTHENES_TEST