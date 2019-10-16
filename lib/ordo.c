#include "ordo.h"

void print_prio(void)
{
	int prio_proc = 0;

	errno	  = 0;
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

void set_affinity(int cpu_nb)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu_nb, &mask);

	if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
		errx(EXIT_FAILURE, "sched_setaffinity");
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

void create_timer(__syscall_slong_t inter)
{
	struct sigevent	  sev;
	struct itimerspec its;
	sample_count = 0;

	// create the timer
	sev.sigev_notify	  = SIGEV_SIGNAL;
	sev.sigev_signo		  = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
		errx(EXIT_FAILURE, "timer_create");

	printf("timerid : %p\n", timerid);

	// start the timer
	its.it_value.tv_sec	= 0;
	its.it_value.tv_nsec	= inter;
	its.it_interval.tv_sec	= its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errx(EXIT_FAILURE, "timer_settime");
}

void calculations(void)
{
	// all diffs
	for (int i = 0; i < DIFFS; i++) {
		diffs[i].tv_sec = timings[i + 1].tv_sec - timings[i].tv_sec;
		if (timings[i].tv_nsec > timings[i + 1].tv_nsec) {
			diffs[i].tv_sec--;
			diffs[i].tv_nsec = (1000000000 - timings[i].tv_nsec) +
					   timings[i + 1].tv_nsec;
		} else
			diffs[i].tv_nsec =
				timings[i + 1].tv_nsec - timings[i].tv_nsec;
	}

	// mean
	__syscall_slong_t mean = 0;
	__syscall_slong_t sum  = 0;
	for (int i = 0; i < DIFFS; i++)
		sum += diffs[i].tv_nsec;
	mean = sum / DIFFS;
	printf("mean : %ld ns\n", mean);

	// standard deviation
	__syscall_slong_t std_dev = 0;
	for (int i = 0; i < DIFFS; ++i)
		std_dev += pow(diffs[i].tv_nsec - mean, 2);
	std_dev = sqrt(std_dev / DIFFS);
	printf("std dev : %ld ns\n", std_dev);
}

inline void sample_while(void)
{
	while (1) {
		if (break_while)
			break;
	}
}