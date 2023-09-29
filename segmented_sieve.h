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

#ifndef SEGMENTED_SIEVE_H
#define SEGMENTED_SIEVE_H

#include <pthread.h>
#include <stdbool.h>

#define PARTITION_SIZE(a, b) (a / b)
#define RANGE_START(a, b) a *b
#define RANGE_END(a, b) a *(b + 1)

struct shared_params_t {
	int max_threads;
	int n_threads;
	int start;
	int end;
	bool *prime;
	int *help_list;
	int help_list_size;
	pthread_cond_t t_cond;
	pthread_mutex_t t_mutex;
};

struct tparams_t {
	struct shared_params_t *shared;
	int start;
	int end;
};

bool *ss_find_primes(int start, int end, int max_threads);

#endif