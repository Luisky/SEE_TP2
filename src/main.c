#include "ordo.h"

#define N 100

int     timer_count = 0;
timer_t timerid; // pour pouvoir arreter le timer dans le handler

struct timespec timings[N];

static void handler(int sig, siginfo_t *si, void *uc)
{
	/* Note: calling printf() from a signal handler is not safe
              (and should not be done in production programs), since
              printf() is not async-signal-safe; see signal-safety(7).
              Nevertheless, we use printf() here as a simple way of
              showing that the handler was called. */

	//printf("%d : Caught signal %d\n", timer_count++, sig);
	if (timer_count >= N)
		timer_delete(timerid);
	clock_gettime(CLOCK_REALTIME, &timings[timer_count]);
	timer_count++;
}

int main(void)
{
	// questions 1, 2, 3
	set_affinity(0);
	print_affinity();

	print_prio();
	set_prio(-10);
	print_prio();

	/* TODO: revert this back for question 5
	change_sched_to_rt(50);
	print_sched_policy();
	print_rt_prio();*/

	// question 4

	struct sigevent   sev;
	struct itimerspec its;
	struct sigaction  sa;

	// copied from man page : establish handler for timer signal
	sa.sa_flags     = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGRTMIN, &sa, NULL) == -1)
		errx(EXIT_FAILURE, "sigaction");

	// create the timer
	sev.sigev_notify	  = SIGEV_SIGNAL;
	sev.sigev_signo		  = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
		errx(EXIT_FAILURE, "timer_create");

	// start the timer
	its.it_value.tv_sec     = 0;
	its.it_value.tv_nsec    = 100000000;
	its.it_interval.tv_sec  = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errx(EXIT_FAILURE, "timer_settime");

	/*
	for (int i = 0; i < 10; i++)
		sleep(1); // for testing the timer
	*/

	while (1) {
		if (timer_count >= N)
			break;
	}

	printf("timer count : %d\n", timer_count);

	// calcul du temps moyen
	for (int i = 0; i < N - 1; i++) {
		__time_t	  tv_sec_res  = 0;
		__syscall_slong_t tv_nsec_res = 0;

		tv_sec_res = timings[i + 1].tv_sec - timings[i].tv_sec;
		if (timings[i].tv_nsec > timings[i + 1].tv_nsec) {
			tv_sec_res--;
			tv_nsec_res = (1000000000 - timings[i].tv_nsec) +
				      timings[i + 1].tv_nsec;
		} else
			tv_nsec_res =
				timings[i + 1].tv_nsec - timings[i].tv_nsec;

		printf("%d & %d : %ld\n", i, i + 1,
		       ((tv_sec_res * 1000000) + (tv_nsec_res / 1000)));
	}

	return EXIT_SUCCESS;
}