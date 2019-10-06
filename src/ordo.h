#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sched.h>
#include <err.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

// questions 1,2,3
void print_prio(void);
void set_prio(int prio);
void change_sched_to_rt(int prio);
void print_affinity(void);
void print_sched_policy(void);
void print_rt_prio(void);