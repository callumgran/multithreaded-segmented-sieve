/*
 *  Copyright (C) 2023 Callum Gran
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "segmented_sieve.h"

bool* sieve_of_eratosthenes(int end) {
    bool* prime = (bool*)malloc((end + 1) * sizeof(bool));
	memset(prime, true, (end + 1) * sizeof(bool));

    *prime = false;
	*(prime + 1) = false;

    for (int p = 2; p * p <= end; p++) {
        if (*(prime + p) == true) {
            for (int i = p * p; i <= end; i += p) {
                *(prime + i) = false;
            }
        }
    }

    return prime;
}

void print_list(bool *primes, int start, int end)
{
	for (int i = start; i <= end; i++)
		if (*(primes + i - start))
			printf("%d ", i);

	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc == 4) {
		int start = atoi(*(argv + 1));
		int end = atoi(*(argv + 2));
		int threads = atoi(*(argv + 3));

		struct timespec t_start, finish;

		clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
		bool *primes = ss_find_primes(start, end, threads);
		clock_gettime(CLOCK_MONOTONIC_RAW, &finish);

		double elapsed = (finish.tv_sec - t_start.tv_sec);
		elapsed += (finish.tv_nsec - t_start.tv_nsec) / 1000000000.0;

#ifdef PRINT_PRIMES
		print_list(primes, start, end);
#endif

		printf("Time used was %fms for the numbers between %d and %d. \n", elapsed * 1000, start,
			   end);

		free(primes);

#ifdef ERATOSTHENES_TEST
		clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
		primes = sieve_of_eratosthenes(end);
		clock_gettime(CLOCK_MONOTONIC_RAW, &finish);

		elapsed = (finish.tv_sec - t_start.tv_sec);
		elapsed += (finish.tv_nsec - t_start.tv_nsec) / 1000000000.0;

		printf("Time used was %fms for the numbers between up to %d with regular sieve. \n", elapsed * 1000, end);
#endif


	} else {
		fprintf(stderr, "The input was formatted incorrectly, it should be: "
						"./segmented_sieve <start> <end> <threads>\n");
		exit(1);
	}
	return 0;
}