#include "ordo.h"

static void print_siginfo(siginfo_t *si)
{
	timer_t *tidp;
	int or ;

	tidp = si->si_value.sival_ptr;

	printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
	printf("    *sival_ptr = 0x%lx\n", (long)*tidp);

	or = timer_getoverrun(*tidp);
	if (or == -1) {
		perror("timer_getoverrun");
		exit(EXIT_FAILURE);
	} else
		printf("    overrun count = %d\n", or);
}

static void handler(int sig, siginfo_t *si, void *uc)
{
	/* Note: calling printf() from a signal handler is not safe
              (and should not be done in production programs), since
              printf() is not async-signal-safe; see signal-safety(7).
              Nevertheless, we use printf() here as a simple way of
              showing that the handler was called. */

	printf("Caught signal %d\n", sig);
	print_siginfo(si);
	//signal(sig, SIG_IGN);
}

int main(void)
{
	// questions 1, 2, 3
	print_affinity();

	print_prio();
	set_prio(-10);
	print_prio();

	/* TODO: revert this back for question 5
	change_sched_to_rt(50);
	print_sched_policy();
	print_rt_prio();*/

	// question 4

	timer_t  timerid;
	sigset_t mask;

	struct sigevent   sev;
	struct itimerspec its;
	struct sigaction  sa;

	// copied from man page : establish handler for timer signal
	sa.sa_flags     = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGRTMIN, &sa, NULL) == -1)
		errx(EXIT_FAILURE, "sigaction");

	// block timer signal temporarily
	/*sigemptyset(&mask);
	sigaddset(&mask, SIGRTMIN);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		errx(EXIT_FAILURE, "sigprocmask");*/

	// create the timer
	sev.sigev_notify	  = SIGEV_SIGNAL;
	sev.sigev_signo		  = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
		errx(EXIT_FAILURE, "timer_create");

	// start the timer
	its.it_value.tv_sec     = 1;
	its.it_value.tv_nsec    = 0;
	its.it_interval.tv_sec  = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errx(EXIT_FAILURE, "timer_settime");

	for (int i = 0; i < 10; i++)
		sleep(1); // for testing the timer

	// unlock signal
	/*if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
		errx(EXIT_FAILURE, "sigprocmask");*/

	return EXIT_SUCCESS;
}