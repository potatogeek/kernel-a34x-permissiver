// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2016, Linaro Ltd - Daniel Lezcano <daniel.lezcano@linaro.org>
<<<<<<< HEAD
=======
#define pr_fmt(fmt) "irq_timings: " fmt
>>>>>>> upstream/android-13

#include <linux/kernel.h>
#include <linux/percpu.h>
#include <linux/slab.h>
#include <linux/static_key.h>
<<<<<<< HEAD
=======
#include <linux/init.h>
>>>>>>> upstream/android-13
#include <linux/interrupt.h>
#include <linux/idr.h>
#include <linux/irq.h>
#include <linux/math64.h>
<<<<<<< HEAD
=======
#include <linux/log2.h>
>>>>>>> upstream/android-13

#include <trace/events/irq.h>

#include "internals.h"

DEFINE_STATIC_KEY_FALSE(irq_timing_enabled);

DEFINE_PER_CPU(struct irq_timings, irq_timings);

<<<<<<< HEAD
struct irqt_stat {
	u64	next_evt;
	u64	last_ts;
	u64	variance;
	u32	avg;
	u32	nr_samples;
	int	anomalies;
	int	valid;
};

=======
>>>>>>> upstream/android-13
static DEFINE_IDR(irqt_stats);

void irq_timings_enable(void)
{
	static_branch_enable(&irq_timing_enabled);
}

void irq_timings_disable(void)
{
	static_branch_disable(&irq_timing_enabled);
}

<<<<<<< HEAD
/**
 * irqs_update - update the irq timing statistics with a new timestamp
 *
 * @irqs: an irqt_stat struct pointer
 * @ts: the new timestamp
 *
 * The statistics are computed online, in other words, the code is
 * designed to compute the statistics on a stream of values rather
 * than doing multiple passes on the values to compute the average,
 * then the variance. The integer division introduces a loss of
 * precision but with an acceptable error margin regarding the results
 * we would have with the double floating precision: we are dealing
 * with nanosec, so big numbers, consequently the mantisse is
 * negligeable, especially when converting the time in usec
 * afterwards.
 *
 * The computation happens at idle time. When the CPU is not idle, the
 * interrupts' timestamps are stored in the circular buffer, when the
 * CPU goes idle and this routine is called, all the buffer's values
 * are injected in the statistical model continuying to extend the
 * statistics from the previous busy-idle cycle.
 *
 * The observations showed a device will trigger a burst of periodic
 * interrupts followed by one or two peaks of longer time, for
 * instance when a SD card device flushes its cache, then the periodic
 * intervals occur again. A one second inactivity period resets the
 * stats, that gives us the certitude the statistical values won't
 * exceed 1x10^9, thus the computation won't overflow.
 *
 * Basically, the purpose of the algorithm is to watch the periodic
 * interrupts and eliminate the peaks.
 *
 * An interrupt is considered periodically stable if the interval of
 * its occurences follow the normal distribution, thus the values
 * comply with:
 *
 *      avg - 3 x stddev < value < avg + 3 x stddev
 *
 * Which can be simplified to:
 *
 *      -3 x stddev < value - avg < 3 x stddev
 *
 *      abs(value - avg) < 3 x stddev
 *
 * In order to save a costly square root computation, we use the
 * variance. For the record, stddev = sqrt(variance). The equation
 * above becomes:
 *
 *      abs(value - avg) < 3 x sqrt(variance)
 *
 * And finally we square it:
 *
 *      (value - avg) ^ 2 < (3 x sqrt(variance)) ^ 2
 *
 *      (value - avg) x (value - avg) < 9 x variance
 *
 * Statistically speaking, any values out of this interval is
 * considered as an anomaly and is discarded. However, a normal
 * distribution appears when the number of samples is 30 (it is the
 * rule of thumb in statistics, cf. "30 samples" on Internet). When
 * there are three consecutive anomalies, the statistics are resetted.
 *
 */
static void irqs_update(struct irqt_stat *irqs, u64 ts)
{
	u64 old_ts = irqs->last_ts;
	u64 variance = 0;
	u64 interval;
	s64 diff;
=======
/*
 * The main goal of this algorithm is to predict the next interrupt
 * occurrence on the current CPU.
 *
 * Currently, the interrupt timings are stored in a circular array
 * buffer every time there is an interrupt, as a tuple: the interrupt
 * number and the associated timestamp when the event occurred <irq,
 * timestamp>.
 *
 * For every interrupt occurring in a short period of time, we can
 * measure the elapsed time between the occurrences for the same
 * interrupt and we end up with a suite of intervals. The experience
 * showed the interrupts are often coming following a periodic
 * pattern.
 *
 * The objective of the algorithm is to find out this periodic pattern
 * in a fastest way and use its period to predict the next irq event.
 *
 * When the next interrupt event is requested, we are in the situation
 * where the interrupts are disabled and the circular buffer
 * containing the timings is filled with the events which happened
 * after the previous next-interrupt-event request.
 *
 * At this point, we read the circular buffer and we fill the irq
 * related statistics structure. After this step, the circular array
 * containing the timings is empty because all the values are
 * dispatched in their corresponding buffers.
 *
 * Now for each interrupt, we can predict the next event by using the
 * suffix array, log interval and exponential moving average
 *
 * 1. Suffix array
 *
 * Suffix array is an array of all the suffixes of a string. It is
 * widely used as a data structure for compression, text search, ...
 * For instance for the word 'banana', the suffixes will be: 'banana'
 * 'anana' 'nana' 'ana' 'na' 'a'
 *
 * Usually, the suffix array is sorted but for our purpose it is
 * not necessary and won't provide any improvement in the context of
 * the solved problem where we clearly define the boundaries of the
 * search by a max period and min period.
 *
 * The suffix array will build a suite of intervals of different
 * length and will look for the repetition of each suite. If the suite
 * is repeating then we have the period because it is the length of
 * the suite whatever its position in the buffer.
 *
 * 2. Log interval
 *
 * We saw the irq timings allow to compute the interval of the
 * occurrences for a specific interrupt. We can reasonably assume the
 * longer is the interval, the higher is the error for the next event
 * and we can consider storing those interval values into an array
 * where each slot in the array correspond to an interval at the power
 * of 2 of the index. For example, index 12 will contain values
 * between 2^11 and 2^12.
 *
 * At the end we have an array of values where at each index defines a
 * [2^index - 1, 2 ^ index] interval values allowing to store a large
 * number of values inside a small array.
 *
 * For example, if we have the value 1123, then we store it at
 * ilog2(1123) = 10 index value.
 *
 * Storing those value at the specific index is done by computing an
 * exponential moving average for this specific slot. For instance,
 * for values 1800, 1123, 1453, ... fall under the same slot (10) and
 * the exponential moving average is computed every time a new value
 * is stored at this slot.
 *
 * 3. Exponential Moving Average
 *
 * The EMA is largely used to track a signal for stocks or as a low
 * pass filter. The magic of the formula, is it is very simple and the
 * reactivity of the average can be tuned with the factors called
 * alpha.
 *
 * The higher the alphas are, the faster the average respond to the
 * signal change. In our case, if a slot in the array is a big
 * interval, we can have numbers with a big difference between
 * them. The impact of those differences in the average computation
 * can be tuned by changing the alpha value.
 *
 *
 *  -- The algorithm --
 *
 * We saw the different processing above, now let's see how they are
 * used together.
 *
 * For each interrupt:
 *	For each interval:
 *		Compute the index = ilog2(interval)
 *		Compute a new_ema(buffer[index], interval)
 *		Store the index in a circular buffer
 *
 *	Compute the suffix array of the indexes
 *
 *	For each suffix:
 *		If the suffix is reverse-found 3 times
 *			Return suffix
 *
 *	Return Not found
 *
 * However we can not have endless suffix array to be build, it won't
 * make sense and it will add an extra overhead, so we can restrict
 * this to a maximum suffix length of 5 and a minimum suffix length of
 * 2. The experience showed 5 is the majority of the maximum pattern
 * period found for different devices.
 *
 * The result is a pattern finding less than 1us for an interrupt.
 *
 * Example based on real values:
 *
 * Example 1 : MMC write/read interrupt interval:
 *
 *	223947, 1240, 1384, 1386, 1386,
 *	217416, 1236, 1384, 1386, 1387,
 *	214719, 1241, 1386, 1387, 1384,
 *	213696, 1234, 1384, 1386, 1388,
 *	219904, 1240, 1385, 1389, 1385,
 *	212240, 1240, 1386, 1386, 1386,
 *	214415, 1236, 1384, 1386, 1387,
 *	214276, 1234, 1384, 1388, ?
 *
 * For each element, apply ilog2(value)
 *
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, ?
 *
 * Max period of 5, we take the last (max_period * 3) 15 elements as
 * we can be confident if the pattern repeats itself three times it is
 * a repeating pattern.
 *
 *	             8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, 8,
 *	15, 8, 8, 8, ?
 *
 * Suffixes are:
 *
 *  1) 8, 15, 8, 8, 8  <- max period
 *  2) 8, 15, 8, 8
 *  3) 8, 15, 8
 *  4) 8, 15           <- min period
 *
 * From there we search the repeating pattern for each suffix.
 *
 * buffer: 8, 15, 8, 8, 8, 8, 15, 8, 8, 8, 8, 15, 8, 8, 8
 *         |   |  |  |  |  |   |  |  |  |  |   |  |  |  |
 *         8, 15, 8, 8, 8  |   |  |  |  |  |   |  |  |  |
 *                         8, 15, 8, 8, 8  |   |  |  |  |
 *                                         8, 15, 8, 8, 8
 *
 * When moving the suffix, we found exactly 3 matches.
 *
 * The first suffix with period 5 is repeating.
 *
 * The next event is (3 * max_period) % suffix_period
 *
 * In this example, the result 0, so the next event is suffix[0] => 8
 *
 * However, 8 is the index in the array of exponential moving average
 * which was calculated on the fly when storing the values, so the
 * interval is ema[8] = 1366
 *
 *
 * Example 2:
 *
 *	4, 3, 5, 100,
 *	3, 3, 5, 117,
 *	4, 4, 5, 112,
 *	4, 3, 4, 110,
 *	3, 5, 3, 117,
 *	4, 4, 5, 112,
 *	4, 3, 4, 110,
 *	3, 4, 5, 112,
 *	4, 3, 4, 110
 *
 * ilog2
 *
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4
 *
 * Max period 5:
 *	   0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4,
 *	0, 0, 0, 4
 *
 * Suffixes:
 *
 *  1) 0, 0, 4, 0, 0
 *  2) 0, 0, 4, 0
 *  3) 0, 0, 4
 *  4) 0, 0
 *
 * buffer: 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4
 *         |  |  |  |  |  |  X
 *         0, 0, 4, 0, 0, |  X
 *                        0, 0
 *
 * buffer: 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 4
 *         |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *         0, 0, 4, 0, |  |  |  |  |  |  |  |  |  |  |
 *                     0, 0, 4, 0, |  |  |  |  |  |  |
 *                                 0, 0, 4, 0, |  |  |
 *                                             0  0  4
 *
 * Pattern is found 3 times, the remaining is 1 which results from
 * (max_period * 3) % suffix_period. This value is the index in the
 * suffix arrays. The suffix array for a period 4 has the value 4
 * at index 1.
 */
#define EMA_ALPHA_VAL		64
#define EMA_ALPHA_SHIFT		7

#define PREDICTION_PERIOD_MIN	3
#define PREDICTION_PERIOD_MAX	5
#define PREDICTION_FACTOR	4
#define PREDICTION_MAX		10 /* 2 ^ PREDICTION_MAX useconds */
#define PREDICTION_BUFFER_SIZE	16 /* slots for EMAs, hardly more than 16 */

/*
 * Number of elements in the circular buffer: If it happens it was
 * flushed before, then the number of elements could be smaller than
 * IRQ_TIMINGS_SIZE, so the count is used, otherwise the array size is
 * used as we wrapped. The index begins from zero when we did not
 * wrap. That could be done in a nicer way with the proper circular
 * array structure type but with the cost of extra computation in the
 * interrupt handler hot path. We choose efficiency.
 */
#define for_each_irqts(i, irqts)					\
	for (i = irqts->count < IRQ_TIMINGS_SIZE ?			\
		     0 : irqts->count & IRQ_TIMINGS_MASK,		\
		     irqts->count = min(IRQ_TIMINGS_SIZE,		\
					irqts->count);			\
	     irqts->count > 0; irqts->count--,				\
		     i = (i + 1) & IRQ_TIMINGS_MASK)

struct irqt_stat {
	u64	last_ts;
	u64	ema_time[PREDICTION_BUFFER_SIZE];
	int	timings[IRQ_TIMINGS_SIZE];
	int	circ_timings[IRQ_TIMINGS_SIZE];
	int	count;
};

/*
 * Exponential moving average computation
 */
static u64 irq_timings_ema_new(u64 value, u64 ema_old)
{
	s64 diff;

	if (unlikely(!ema_old))
		return value;

	diff = (value - ema_old) * EMA_ALPHA_VAL;
	/*
	 * We can use a s64 type variable to be added with the u64
	 * ema_old variable as this one will never have its topmost
	 * bit set, it will be always smaller than 2^63 nanosec
	 * interrupt interval (292 years).
	 */
	return ema_old + (diff >> EMA_ALPHA_SHIFT);
}

static int irq_timings_next_event_index(int *buffer, size_t len, int period_max)
{
	int period;

	/*
	 * Move the beginning pointer to the end minus the max period x 3.
	 * We are at the point we can begin searching the pattern
	 */
	buffer = &buffer[len - (period_max * 3)];

	/* Adjust the length to the maximum allowed period x 3 */
	len = period_max * 3;

	/*
	 * The buffer contains the suite of intervals, in a ilog2
	 * basis, we are looking for a repetition. We point the
	 * beginning of the search three times the length of the
	 * period beginning at the end of the buffer. We do that for
	 * each suffix.
	 */
	for (period = period_max; period >= PREDICTION_PERIOD_MIN; period--) {

		/*
		 * The first comparison always succeed because the
		 * suffix is deduced from the first n-period bytes of
		 * the buffer and we compare the initial suffix with
		 * itself, so we can skip the first iteration.
		 */
		int idx = period;
		size_t size = period;

		/*
		 * We look if the suite with period 'i' repeat
		 * itself. If it is truncated at the end, as it
		 * repeats we can use the period to find out the next
		 * element with the modulo.
		 */
		while (!memcmp(buffer, &buffer[idx], size * sizeof(int))) {

			/*
			 * Move the index in a period basis
			 */
			idx += size;

			/*
			 * If this condition is reached, all previous
			 * memcmp were successful, so the period is
			 * found.
			 */
			if (idx == len)
				return buffer[len % period];

			/*
			 * If the remaining elements to compare are
			 * smaller than the period, readjust the size
			 * of the comparison for the last iteration.
			 */
			if (len - idx < period)
				size = len - idx;
		}
	}

	return -1;
}

static u64 __irq_timings_next_event(struct irqt_stat *irqs, int irq, u64 now)
{
	int index, i, period_max, count, start, min = INT_MAX;

	if ((now - irqs->last_ts) >= NSEC_PER_SEC) {
		irqs->count = irqs->last_ts = 0;
		return U64_MAX;
	}

	/*
	 * As we want to find three times the repetition, we need a
	 * number of intervals greater or equal to three times the
	 * maximum period, otherwise we truncate the max period.
	 */
	period_max = irqs->count > (3 * PREDICTION_PERIOD_MAX) ?
		PREDICTION_PERIOD_MAX : irqs->count / 3;

	/*
	 * If we don't have enough irq timings for this prediction,
	 * just bail out.
	 */
	if (period_max <= PREDICTION_PERIOD_MIN)
		return U64_MAX;

	/*
	 * 'count' will depends if the circular buffer wrapped or not
	 */
	count = irqs->count < IRQ_TIMINGS_SIZE ?
		irqs->count : IRQ_TIMINGS_SIZE;

	start = irqs->count < IRQ_TIMINGS_SIZE ?
		0 : (irqs->count & IRQ_TIMINGS_MASK);

	/*
	 * Copy the content of the circular buffer into another buffer
	 * in order to linearize the buffer instead of dealing with
	 * wrapping indexes and shifted array which will be prone to
	 * error and extremely difficult to debug.
	 */
	for (i = 0; i < count; i++) {
		int index = (start + i) & IRQ_TIMINGS_MASK;

		irqs->timings[i] = irqs->circ_timings[index];
		min = min_t(int, irqs->timings[i], min);
	}

	index = irq_timings_next_event_index(irqs->timings, count, period_max);
	if (index < 0)
		return irqs->last_ts + irqs->ema_time[min];

	return irqs->last_ts + irqs->ema_time[index];
}

static __always_inline int irq_timings_interval_index(u64 interval)
{
	/*
	 * The PREDICTION_FACTOR increase the interval size for the
	 * array of exponential average.
	 */
	u64 interval_us = (interval >> 10) / PREDICTION_FACTOR;

	return likely(interval_us) ? ilog2(interval_us) : 0;
}

static __always_inline void __irq_timings_store(int irq, struct irqt_stat *irqs,
						u64 interval)
{
	int index;

	/*
	 * Get the index in the ema table for this interrupt.
	 */
	index = irq_timings_interval_index(interval);

	if (index > PREDICTION_BUFFER_SIZE - 1) {
		irqs->count = 0;
		return;
	}

	/*
	 * Store the index as an element of the pattern in another
	 * circular array.
	 */
	irqs->circ_timings[irqs->count & IRQ_TIMINGS_MASK] = index;

	irqs->ema_time[index] = irq_timings_ema_new(interval,
						    irqs->ema_time[index]);

	irqs->count++;
}

static inline void irq_timings_store(int irq, struct irqt_stat *irqs, u64 ts)
{
	u64 old_ts = irqs->last_ts;
	u64 interval;
>>>>>>> upstream/android-13

	/*
	 * The timestamps are absolute time values, we need to compute
	 * the timing interval between two interrupts.
	 */
	irqs->last_ts = ts;

	/*
	 * The interval type is u64 in order to deal with the same
	 * type in our computation, that prevent mindfuck issues with
	 * overflow, sign and division.
	 */
	interval = ts - old_ts;

	/*
	 * The interrupt triggered more than one second apart, that
<<<<<<< HEAD
	 * ends the sequence as predictible for our purpose. In this
=======
	 * ends the sequence as predictable for our purpose. In this
>>>>>>> upstream/android-13
	 * case, assume we have the beginning of a sequence and the
	 * timestamp is the first value. As it is impossible to
	 * predict anything at this point, return.
	 *
	 * Note the first timestamp of the sequence will always fall
	 * in this test because the old_ts is zero. That is what we
	 * want as we need another timestamp to compute an interval.
	 */
	if (interval >= NSEC_PER_SEC) {
<<<<<<< HEAD
		memset(irqs, 0, sizeof(*irqs));
		irqs->last_ts = ts;
		return;
	}

	/*
	 * Pre-compute the delta with the average as the result is
	 * used several times in this function.
	 */
	diff = interval - irqs->avg;

	/*
	 * Increment the number of samples.
	 */
	irqs->nr_samples++;

	/*
	 * Online variance divided by the number of elements if there
	 * is more than one sample.  Normally the formula is division
	 * by nr_samples - 1 but we assume the number of element will be
	 * more than 32 and dividing by 32 instead of 31 is enough
	 * precise.
	 */
	if (likely(irqs->nr_samples > 1))
		variance = irqs->variance >> IRQ_TIMINGS_SHIFT;

	/*
	 * The rule of thumb in statistics for the normal distribution
	 * is having at least 30 samples in order to have the model to
	 * apply. Values outside the interval are considered as an
	 * anomaly.
	 */
	if ((irqs->nr_samples >= 30) && ((diff * diff) > (9 * variance))) {
		/*
		 * After three consecutive anomalies, we reset the
		 * stats as it is no longer stable enough.
		 */
		if (irqs->anomalies++ >= 3) {
			memset(irqs, 0, sizeof(*irqs));
			irqs->last_ts = ts;
			return;
		}
	} else {
		/*
		 * The anomalies must be consecutives, so at this
		 * point, we reset the anomalies counter.
		 */
		irqs->anomalies = 0;
	}

	/*
	 * The interrupt is considered stable enough to try to predict
	 * the next event on it.
	 */
	irqs->valid = 1;

	/*
	 * Online average algorithm:
	 *
	 *  new_average = average + ((value - average) / count)
	 *
	 * The variance computation depends on the new average
	 * to be computed here first.
	 *
	 */
	irqs->avg = irqs->avg + (diff >> IRQ_TIMINGS_SHIFT);

	/*
	 * Online variance algorithm:
	 *
	 *  new_variance = variance + (value - average) x (value - new_average)
	 *
	 * Warning: irqs->avg is updated with the line above, hence
	 * 'interval - irqs->avg' is no longer equal to 'diff'
	 */
	irqs->variance = irqs->variance + (diff * (interval - irqs->avg));

	/*
	 * Update the next event
	 */
	irqs->next_evt = ts + irqs->avg;
=======
		irqs->count = 0;
		return;
	}

	__irq_timings_store(irq, irqs, interval);
>>>>>>> upstream/android-13
}

/**
 * irq_timings_next_event - Return when the next event is supposed to arrive
 *
 * During the last busy cycle, the number of interrupts is incremented
 * and stored in the irq_timings structure. This information is
 * necessary to:
 *
 * - know if the index in the table wrapped up:
 *
 *      If more than the array size interrupts happened during the
 *      last busy/idle cycle, the index wrapped up and we have to
 *      begin with the next element in the array which is the last one
<<<<<<< HEAD
 *      in the sequence, otherwise it is a the index 0.
=======
 *      in the sequence, otherwise it is at the index 0.
>>>>>>> upstream/android-13
 *
 * - have an indication of the interrupts activity on this CPU
 *   (eg. irq/sec)
 *
 * The values are 'consumed' after inserting in the statistical model,
 * thus the count is reinitialized.
 *
 * The array of values **must** be browsed in the time direction, the
 * timestamp must increase between an element and the next one.
 *
 * Returns a nanosec time based estimation of the earliest interrupt,
 * U64_MAX otherwise.
 */
u64 irq_timings_next_event(u64 now)
{
	struct irq_timings *irqts = this_cpu_ptr(&irq_timings);
	struct irqt_stat *irqs;
	struct irqt_stat __percpu *s;
	u64 ts, next_evt = U64_MAX;
	int i, irq = 0;

	/*
	 * This function must be called with the local irq disabled in
	 * order to prevent the timings circular buffer to be updated
	 * while we are reading it.
	 */
	lockdep_assert_irqs_disabled();

<<<<<<< HEAD
=======
	if (!irqts->count)
		return next_evt;

>>>>>>> upstream/android-13
	/*
	 * Number of elements in the circular buffer: If it happens it
	 * was flushed before, then the number of elements could be
	 * smaller than IRQ_TIMINGS_SIZE, so the count is used,
	 * otherwise the array size is used as we wrapped. The index
	 * begins from zero when we did not wrap. That could be done
	 * in a nicer way with the proper circular array structure
	 * type but with the cost of extra computation in the
	 * interrupt handler hot path. We choose efficiency.
	 *
<<<<<<< HEAD
	 * Inject measured irq/timestamp to the statistical model
	 * while decrementing the counter because we consume the data
	 * from our circular buffer.
	 */
	for (i = irqts->count & IRQ_TIMINGS_MASK,
		     irqts->count = min(IRQ_TIMINGS_SIZE, irqts->count);
	     irqts->count > 0; irqts->count--, i = (i + 1) & IRQ_TIMINGS_MASK) {

		irq = irq_timing_decode(irqts->values[i], &ts);

		s = idr_find(&irqt_stats, irq);
		if (s) {
			irqs = this_cpu_ptr(s);
			irqs_update(irqs, ts);
		}
=======
	 * Inject measured irq/timestamp to the pattern prediction
	 * model while decrementing the counter because we consume the
	 * data from our circular buffer.
	 */
	for_each_irqts(i, irqts) {
		irq = irq_timing_decode(irqts->values[i], &ts);
		s = idr_find(&irqt_stats, irq);
		if (s)
			irq_timings_store(irq, this_cpu_ptr(s), ts);
>>>>>>> upstream/android-13
	}

	/*
	 * Look in the list of interrupts' statistics, the earliest
	 * next event.
	 */
	idr_for_each_entry(&irqt_stats, s, i) {

		irqs = this_cpu_ptr(s);

<<<<<<< HEAD
		if (!irqs->valid)
			continue;

		if (irqs->next_evt <= now) {
			irq = i;
			next_evt = now;

			/*
			 * This interrupt mustn't use in the future
			 * until new events occur and update the
			 * statistics.
			 */
			irqs->valid = 0;
			break;
		}

		if (irqs->next_evt < next_evt) {
			irq = i;
			next_evt = irqs->next_evt;
		}
=======
		ts = __irq_timings_next_event(irqs, i, now);
		if (ts <= now)
			return now;

		if (ts < next_evt)
			next_evt = ts;
>>>>>>> upstream/android-13
	}

	return next_evt;
}

void irq_timings_free(int irq)
{
	struct irqt_stat __percpu *s;

	s = idr_find(&irqt_stats, irq);
	if (s) {
		free_percpu(s);
		idr_remove(&irqt_stats, irq);
	}
}

int irq_timings_alloc(int irq)
{
	struct irqt_stat __percpu *s;
	int id;

	/*
	 * Some platforms can have the same private interrupt per cpu,
<<<<<<< HEAD
	 * so this function may be be called several times with the
=======
	 * so this function may be called several times with the
>>>>>>> upstream/android-13
	 * same interrupt number. Just bail out in case the per cpu
	 * stat structure is already allocated.
	 */
	s = idr_find(&irqt_stats, irq);
	if (s)
		return 0;

	s = alloc_percpu(*s);
	if (!s)
		return -ENOMEM;

	idr_preload(GFP_KERNEL);
	id = idr_alloc(&irqt_stats, s, irq, irq + 1, GFP_NOWAIT);
	idr_preload_end();

	if (id < 0) {
		free_percpu(s);
		return id;
	}

	return 0;
}
<<<<<<< HEAD
=======

#ifdef CONFIG_TEST_IRQ_TIMINGS
struct timings_intervals {
	u64 *intervals;
	size_t count;
};

/*
 * Intervals are given in nanosecond base
 */
static u64 intervals0[] __initdata = {
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000, 500000,
	10000, 50000, 200000,
};

static u64 intervals1[] __initdata = {
	223947000, 1240000, 1384000, 1386000, 1386000,
	217416000, 1236000, 1384000, 1386000, 1387000,
	214719000, 1241000, 1386000, 1387000, 1384000,
	213696000, 1234000, 1384000, 1386000, 1388000,
	219904000, 1240000, 1385000, 1389000, 1385000,
	212240000, 1240000, 1386000, 1386000, 1386000,
	214415000, 1236000, 1384000, 1386000, 1387000,
	214276000, 1234000,
};

static u64 intervals2[] __initdata = {
	4000, 3000, 5000, 100000,
	3000, 3000, 5000, 117000,
	4000, 4000, 5000, 112000,
	4000, 3000, 4000, 110000,
	3000, 5000, 3000, 117000,
	4000, 4000, 5000, 112000,
	4000, 3000, 4000, 110000,
	3000, 4000, 5000, 112000,
	4000,
};

static u64 intervals3[] __initdata = {
	1385000, 212240000, 1240000,
	1386000, 214415000, 1236000,
	1384000, 214276000, 1234000,
	1386000, 214415000, 1236000,
	1385000, 212240000, 1240000,
	1386000, 214415000, 1236000,
	1384000, 214276000, 1234000,
	1386000, 214415000, 1236000,
	1385000, 212240000, 1240000,
};

static u64 intervals4[] __initdata = {
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000, 50000, 10000, 50000,
	10000,
};

static struct timings_intervals tis[] __initdata = {
	{ intervals0, ARRAY_SIZE(intervals0) },
	{ intervals1, ARRAY_SIZE(intervals1) },
	{ intervals2, ARRAY_SIZE(intervals2) },
	{ intervals3, ARRAY_SIZE(intervals3) },
	{ intervals4, ARRAY_SIZE(intervals4) },
};

static int __init irq_timings_test_next_index(struct timings_intervals *ti)
{
	int _buffer[IRQ_TIMINGS_SIZE];
	int buffer[IRQ_TIMINGS_SIZE];
	int index, start, i, count, period_max;

	count = ti->count - 1;

	period_max = count > (3 * PREDICTION_PERIOD_MAX) ?
		PREDICTION_PERIOD_MAX : count / 3;

	/*
	 * Inject all values except the last one which will be used
	 * to compare with the next index result.
	 */
	pr_debug("index suite: ");

	for (i = 0; i < count; i++) {
		index = irq_timings_interval_index(ti->intervals[i]);
		_buffer[i & IRQ_TIMINGS_MASK] = index;
		pr_cont("%d ", index);
	}

	start = count < IRQ_TIMINGS_SIZE ? 0 :
		count & IRQ_TIMINGS_MASK;

	count = min_t(int, count, IRQ_TIMINGS_SIZE);

	for (i = 0; i < count; i++) {
		int index = (start + i) & IRQ_TIMINGS_MASK;
		buffer[i] = _buffer[index];
	}

	index = irq_timings_next_event_index(buffer, count, period_max);
	i = irq_timings_interval_index(ti->intervals[ti->count - 1]);

	if (index != i) {
		pr_err("Expected (%d) and computed (%d) next indexes differ\n",
		       i, index);
		return -EINVAL;
	}

	return 0;
}

static int __init irq_timings_next_index_selftest(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(tis); i++) {

		pr_info("---> Injecting intervals number #%d (count=%zd)\n",
			i, tis[i].count);

		ret = irq_timings_test_next_index(&tis[i]);
		if (ret)
			break;
	}

	return ret;
}

static int __init irq_timings_test_irqs(struct timings_intervals *ti)
{
	struct irqt_stat __percpu *s;
	struct irqt_stat *irqs;
	int i, index, ret, irq = 0xACE5;

	ret = irq_timings_alloc(irq);
	if (ret) {
		pr_err("Failed to allocate irq timings\n");
		return ret;
	}

	s = idr_find(&irqt_stats, irq);
	if (!s) {
		ret = -EIDRM;
		goto out;
	}

	irqs = this_cpu_ptr(s);

	for (i = 0; i < ti->count; i++) {

		index = irq_timings_interval_index(ti->intervals[i]);
		pr_debug("%d: interval=%llu ema_index=%d\n",
			 i, ti->intervals[i], index);

		__irq_timings_store(irq, irqs, ti->intervals[i]);
		if (irqs->circ_timings[i & IRQ_TIMINGS_MASK] != index) {
			ret = -EBADSLT;
			pr_err("Failed to store in the circular buffer\n");
			goto out;
		}
	}

	if (irqs->count != ti->count) {
		ret = -ERANGE;
		pr_err("Count differs\n");
		goto out;
	}

	ret = 0;
out:
	irq_timings_free(irq);

	return ret;
}

static int __init irq_timings_irqs_selftest(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(tis); i++) {
		pr_info("---> Injecting intervals number #%d (count=%zd)\n",
			i, tis[i].count);
		ret = irq_timings_test_irqs(&tis[i]);
		if (ret)
			break;
	}

	return ret;
}

static int __init irq_timings_test_irqts(struct irq_timings *irqts,
					 unsigned count)
{
	int start = count >= IRQ_TIMINGS_SIZE ? count - IRQ_TIMINGS_SIZE : 0;
	int i, irq, oirq = 0xBEEF;
	u64 ots = 0xDEAD, ts;

	/*
	 * Fill the circular buffer by using the dedicated function.
	 */
	for (i = 0; i < count; i++) {
		pr_debug("%d: index=%d, ts=%llX irq=%X\n",
			 i, i & IRQ_TIMINGS_MASK, ots + i, oirq + i);

		irq_timings_push(ots + i, oirq + i);
	}

	/*
	 * Compute the first elements values after the index wrapped
	 * up or not.
	 */
	ots += start;
	oirq += start;

	/*
	 * Test the circular buffer count is correct.
	 */
	pr_debug("---> Checking timings array count (%d) is right\n", count);
	if (WARN_ON(irqts->count != count))
		return -EINVAL;

	/*
	 * Test the macro allowing to browse all the irqts.
	 */
	pr_debug("---> Checking the for_each_irqts() macro\n");
	for_each_irqts(i, irqts) {

		irq = irq_timing_decode(irqts->values[i], &ts);

		pr_debug("index=%d, ts=%llX / %llX, irq=%X / %X\n",
			 i, ts, ots, irq, oirq);

		if (WARN_ON(ts != ots || irq != oirq))
			return -EINVAL;

		ots++; oirq++;
	}

	/*
	 * The circular buffer should have be flushed when browsed
	 * with for_each_irqts
	 */
	pr_debug("---> Checking timings array is empty after browsing it\n");
	if (WARN_ON(irqts->count))
		return -EINVAL;

	return 0;
}

static int __init irq_timings_irqts_selftest(void)
{
	struct irq_timings *irqts = this_cpu_ptr(&irq_timings);
	int i, ret;

	/*
	 * Test the circular buffer with different number of
	 * elements. The purpose is to test at the limits (empty, half
	 * full, full, wrapped with the cursor at the boundaries,
	 * wrapped several times, etc ...
	 */
	int count[] = { 0,
			IRQ_TIMINGS_SIZE >> 1,
			IRQ_TIMINGS_SIZE,
			IRQ_TIMINGS_SIZE + (IRQ_TIMINGS_SIZE >> 1),
			2 * IRQ_TIMINGS_SIZE,
			(2 * IRQ_TIMINGS_SIZE) + 3,
	};

	for (i = 0; i < ARRAY_SIZE(count); i++) {

		pr_info("---> Checking the timings with %d/%d values\n",
			count[i], IRQ_TIMINGS_SIZE);

		ret = irq_timings_test_irqts(irqts, count[i]);
		if (ret)
			break;
	}

	return ret;
}

static int __init irq_timings_selftest(void)
{
	int ret;

	pr_info("------------------- selftest start -----------------\n");

	/*
	 * At this point, we don't except any subsystem to use the irq
	 * timings but us, so it should not be enabled.
	 */
	if (static_branch_unlikely(&irq_timing_enabled)) {
		pr_warn("irq timings already initialized, skipping selftest\n");
		return 0;
	}

	ret = irq_timings_irqts_selftest();
	if (ret)
		goto out;

	ret = irq_timings_irqs_selftest();
	if (ret)
		goto out;

	ret = irq_timings_next_index_selftest();
out:
	pr_info("---------- selftest end with %s -----------\n",
		ret ? "failure" : "success");

	return ret;
}
early_initcall(irq_timings_selftest);
#endif
>>>>>>> upstream/android-13
