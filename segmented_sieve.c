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

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "segmented_sieve.h"

struct shared_params_t *shared_params_malloc(int threads, int start, int end)
{
	struct shared_params_t *s_params =
		(struct shared_params_t *)(malloc(sizeof(struct shared_params_t)));
	pthread_cond_init(&s_params->t_cond, NULL);
	pthread_mutex_init(&s_params->t_mutex, NULL);
	s_params->max_threads = threads;
	s_params->n_threads = 0;
	s_params->start = start;
	s_params->end = end;
	s_params->help_list = (int *)(malloc(sizeof(int) * ((int)sqrt(end))));
	return s_params;
}

void shared_params_free(struct shared_params_t *s_params)
{
	pthread_cond_destroy(&s_params->t_cond);
	pthread_mutex_destroy(&s_params->t_mutex);
    free(s_params->help_list);
	free(s_params);
}

void fill_nums(int *list, int *size, int end)
{
	bool *ck = (bool *)(malloc(sizeof(bool) * (end + 1)));
	memset(ck, true, sizeof(bool) * (end + 1));

	*ck = false;
	*(ck + 1) = false;

	int end_sqrt = sqrt(end);

	for (int i = 2; i <= end_sqrt; i++) {
		if (*(ck + i)) {
			for (int j = i * i; j <= end_sqrt; j += i) {
				*(ck + j) = false;
			}
		}
	}

	for (int i = 2; i * i <= end; i++) {
		if (*(ck + i)) {
			*(list + *size) = i;
			(*size)++;
		}
	}

	free(ck);
}

void *sieve(void *arg)
{
	struct tparams_t *params = (struct tparams_t *)arg;

	for (int i = params->start; i < params->end; i++) {
		int lower = params->shared->start / *(params->shared->help_list + i);

		if (lower <= 1) {
			lower = *(params->shared->help_list + i) + *(params->shared->help_list + i);
        } else if (params->shared->start % *(params->shared->help_list + i)) {
			lower = (lower * *(params->shared->help_list + i)) + *(params->shared->help_list + i);
		} else {
			lower = (lower * *(params->shared->help_list + i));
        }

		for (int j = lower; j <= params->shared->end; j = j + *(params->shared->help_list + i)) {
			*(params->shared->prime + j - params->shared->start) = false;
        }
	}

	pthread_mutex_lock(&params->shared->t_mutex);
	if (--params->shared->n_threads == 0)
		pthread_cond_signal(&params->shared->t_cond);
	pthread_mutex_unlock(&params->shared->t_mutex);
	free(arg);

    return NULL;
}

bool *ss_find_primes(int start, int end, int max_threads)
{
	struct shared_params_t *shared = shared_params_malloc(max_threads, start, end);

	if (start < 2 && end >= 2)  {
		start = 2;
    }

	fill_nums(shared->help_list, &shared->help_list_size, end);

	if (shared->help_list_size < max_threads) {
		max_threads = shared->help_list_size;
		shared->max_threads = max_threads;
	}

	bool *prime = (bool *)(malloc(sizeof(bool) * (end - start + 1)));
	memset(prime, true, sizeof(bool) * (end - start + 1));

	shared->prime = prime;

	pthread_t threads[max_threads];

	int partition = PARTITION_SIZE(shared->help_list_size, max_threads);

	int rests[max_threads];
	memset(rests, 0, max_threads * sizeof(int));

	int rest = shared->help_list_size % max_threads;

	if (rest != 0)
		for (int i = 0; i < rest; i++)
			(*(rests + i))++;

	for (int i = 0; i < max_threads; i++) {
		struct tparams_t *tparams = (struct tparams_t *)(malloc(sizeof(struct tparams_t)));

		tparams->shared = shared;

		shared->n_threads++;

		if (i == 0) {
			tparams->start = RANGE_START(partition, i);
			tparams->end = RANGE_END(partition, i) + *(rests + i);
		} else {
			if (*(rests + i)) {
				tparams->start = RANGE_START(partition, i) + i;
				tparams->end = RANGE_END(partition, i) + i;
			} else {
				tparams->start = RANGE_START(partition, i) + rest;
				tparams->end = RANGE_END(partition, i) + rest;
			}
		}

		pthread_create(threads + i, NULL, sieve, tparams);
	}

	pthread_mutex_lock(&shared->t_mutex);
	pthread_cond_wait(&shared->t_cond, &shared->t_mutex);
	pthread_mutex_unlock(&shared->t_mutex);

	shared_params_free(shared);

	return prime;
}