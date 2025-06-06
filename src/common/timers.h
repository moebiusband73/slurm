/*****************************************************************************\
 *  timers.h - timing functions
 *****************************************************************************
 *  Copyright (C) 2002 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov> and Kevin Tew <tew1@llnl.gov>
 *  CODE-OCEC-09-009. All rights reserved.
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _HAVE_TIMERS_H
#define _HAVE_TIMERS_H

#include <sys/time.h>
#include <src/common/slurm_time.h>

#define DEF_TIMERS	struct timeval tv1, tv2; char tv_str[20] = ""; long delta_t;
#define START_TIMER	gettimeofday(&tv1, NULL)
#define END_TIMER do {							\
	gettimeofday(&tv2, NULL);					\
	slurm_diff_tv_str(&tv1, &tv2, tv_str, 20, NULL, 0, &delta_t);	\
} while (0)
#define END_TIMER2(from) do {						\
	gettimeofday(&tv2, NULL);					\
	slurm_diff_tv_str(&tv1, &tv2, tv_str, 20, from, 0, &delta_t);	\
} while (0)
#define END_TIMER3(from, limit) do {					\
	gettimeofday(&tv2, NULL);					\
	slurm_diff_tv_str(&tv1, &tv2, tv_str, 20, from, limit, &delta_t); \
} while (0)
#define DELTA_TIMER	delta_t
#define TIME_STR 	tv_str

/* Return the number of micro-seconds between now and argument "tv",
 * Initialize tv to NOW if zero on entry */
extern int slurm_delta_tv(struct timeval *tv);

/*
 * slurm_diff_tv_str - build a string showing the time difference between two
 *		       times
 * IN tv1 - start of event
 * IN tv2 - end of event
 * OUT tv_str - place to put delta time in format "usec=%ld"
 * IN len_tv_str - size of tv_str in bytes
 * IN from - Name to be printed on long diffs
 * IN limit - limit to wait
 * OUT delta_t - raw time difference in usec
 */
extern void slurm_diff_tv_str(struct timeval *tv1,struct timeval *tv2,
			      char *tv_str, int len_tv_str, const char *from,
			      long limit, long *delta_t);

/* Struct to hold latency metric state */
typedef struct {
	timespec_t total;
	uint64_t count;
	timespec_t last_log;
} latency_metric_t;

typedef struct {
	double avg; /* Average latency in seconds or 0 if not calculated */
	timespec_t delay; /* Delay from START_LATENCY_TIMER() */
} latency_metric_rc_t;

/*
 * Start recording latency metric
 * NOTE: Must have DECL_LATENCY_TIMER() in scope
 * NOTE: call BEGIN_LATENCY_TIMER() instead
 * IN metric - metric state
 * IN start - timestamp to populate
 */
extern void latency_metric_begin(latency_metric_t *metric, timespec_t *start);

/*
 * Stop recording latency metric and perform analysis
 * NOTE: Must have DECL_LATENCY_TIMER() in scope
 * NOTE: call END_LATENCY_TIMER() instead
 * IN metric - metric state
 * IN start - timestamp populated by START_LATENCY_TIMER()
 * IN interval - Min interval between calculating analysis
 * RET struct full of latency metric analysis
 */
extern latency_metric_rc_t latency_metric_end(latency_metric_t *metric,
					      timespec_t *start,
					      const timespec_t interval);

/* Declare latency timer */
#define DECL_LATENCY_TIMER                         \
	static latency_metric_t latency_metric = { \
		.total = { 0, 0 },                 \
		.count = 0,                        \
		.last_log = { 0, 0 },              \
	};                                         \
	static __thread timespec_t latency_metric_start = { 0, 0 };

/* Start latency timer */
#define START_LATENCY_TIMER() \
	latency_metric_begin(&latency_metric, &latency_metric_start)

/* End latency timer and generate analysis if past interval */
#define END_LATENCY_TIMER(interval) \
	latency_metric_end(&latency_metric, &latency_metric_start, interval)

#endif
