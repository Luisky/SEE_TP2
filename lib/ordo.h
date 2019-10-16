#ifndef ORDO_H
#define ORDO_H

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
#include <math.h>
#include <stdbool.h>

#define T_100MS 100000000
#define T_10MS 10000000
#define T_1MS 1000000

#define SAMPLE 100
#define DIFFS (SAMPLE - 1)

int	sample_count;
timer_t timerid; // pour pouvoir arreter le timer dans le handler
bool	break_while;

struct timespec timings[SAMPLE];
struct timespec diffs[DIFFS];

// questions 1,2,3
void print_prio(void);
void set_prio(int prio);
void change_sched_to_rt(int prio);
void print_affinity(void);
void set_affinity(int cpu_nb);
void print_sched_policy(void);
void print_rt_prio(void);

// question 4
void create_timer(__syscall_slong_t inter);
void calculations(void);
void sample_while(void);

#endif //ORDO_H