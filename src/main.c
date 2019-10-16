#include "ordo.h"

static void handler(int sig, siginfo_t *si, void *uc)
{
	/* Note: calling printf() from a signal handler is not safe
              (and should not be done in production programs), since
              printf() is not async-signal-safe; see signal-safety(7).
              Nevertheless, we use printf() here as a simple way of
              showing that the handler was called. */

	if (sample_count == SAMPLE) {
		if (timer_delete(timerid) == -1) {
			perror("timer_delete");
			exit(EXIT_FAILURE);
		}
		break_while = true;
	}
	clock_gettime(CLOCK_REALTIME, &timings[sample_count]);
	sample_count++;
}

int main(void)
{
	// questions 1, 2, 3
	set_affinity(7);
	print_affinity();

	print_prio();
	//set_prio(-10);
	//print_prio();

	//TODO: revert this back for question 5
#ifdef RT
	change_sched_to_rt(1);
	print_sched_policy();
	print_rt_prio();
#endif

	// question 4
	// copied from man page : establish handler for timer signal
	struct sigaction sa;
	sa.sa_flags	= SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGRTMIN, &sa, NULL) == -1)
		errx(EXIT_FAILURE, "sigaction");

	create_timer(T_100MS);
	break_while = false;
	sample_while();
	calculations();

	create_timer(T_10MS);
	break_while = false;
	sample_while();
	calculations();

	create_timer(T_1MS);
	break_while = false;
	sample_while();
	calculations();

	return EXIT_SUCCESS;
}
