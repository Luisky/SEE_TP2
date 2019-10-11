#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <err.h>

int main(void)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(7, &mask);

	if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
		errx(EXIT_FAILURE, "sched_setaffinity");

	int r = 0;
	while (1) {
		r = rand();
		printf("%d\n", r);
		for (int i = 0; i < r; i++) {
			;
		}
		printf("sleeping ...\n");
		usleep(r / 10000);
	}

	return EXIT_SUCCESS;
}