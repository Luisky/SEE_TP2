#include "ordo.h"

void print_prio(void)
{
	int prio_proc = 0;

	errno     = 0;
	prio_proc = getpriority(PRIO_PROCESS, 0);

	if (errno != 0) {
		perror("getpriority");
		exit(EXIT_FAILURE);
	}
	printf("getpriority() proc : %d\n", prio_proc);
}

void set_prio(int prio)
{
	//changes priority
	if (setpriority(PRIO_PROCESS, 0, prio) == -1) {
		perror("setpriority");
		exit(EXIT_FAILURE);
	}
}

void change_sched_to_rt(int prio)
{
	struct sched_param sp;
	sp.sched_priority = prio;
	if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1) {
		perror("sched_setscheduler");
		exit(EXIT_FAILURE);
	}
}

void print_affinity(void)
{
	int nproc = 0;

	cpu_set_t mask; /* Define your cpu_set bit mask. */
	CPU_ZERO(&mask); /* Initialize it all to 0, i.e. no CPUs selected. */
	if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1)
		errx(EXIT_FAILURE, "sched_getaffinity");

	nproc = sysconf(_SC_NPROCESSORS_ONLN);
	printf("sched_getaffinity = ");
	for (int i = 0; i < nproc; i++) {
		printf("%d ", CPU_ISSET(i, &mask));
	}
	printf("\n");
}

void print_sched_policy(void)
{
	int sched_policy = 0;
	// SCHED_OTHER = 0
	// SCHED_FIFO = 1
	sched_policy = sched_getscheduler(0);
	if (sched_policy == -1)
		errx(EXIT_FAILURE, "sched_getscheduler");

	printf("sched_policy : %d\n", sched_policy);
}

void print_rt_prio(void)
{
	struct sched_param sp;
	if (sched_getparam(0, &sp) == -1) {
		perror("sched_getparam");
		exit(EXIT_FAILURE);
	}

	printf("rt_prio : %d\n", sp.sched_priority);
}