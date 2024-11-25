<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#include <linux/bcd.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/mc146818rtc.h>

#ifdef CONFIG_ACPI
#include <linux/acpi.h>
#endif

/*
<<<<<<< HEAD
 * Returns true if a clock update is in progress
 */
static inline unsigned char mc146818_is_updating(void)
{
	unsigned char uip;
	unsigned long flags;

	spin_lock_irqsave(&rtc_lock, flags);
	uip = (CMOS_READ(RTC_FREQ_SELECT) & RTC_UIP);
	spin_unlock_irqrestore(&rtc_lock, flags);
	return uip;
}

unsigned int mc146818_get_time(struct rtc_time *time)
{
	unsigned char ctrl;
	unsigned long flags;
	unsigned char century = 0;
=======
 * If the UIP (Update-in-progress) bit of the RTC is set for more then
 * 10ms, the RTC is apparently broken or not present.
 */
bool mc146818_does_rtc_work(void)
{
	int i;
	unsigned char val;
	unsigned long flags;

	for (i = 0; i < 10; i++) {
		spin_lock_irqsave(&rtc_lock, flags);
		val = CMOS_READ(RTC_FREQ_SELECT);
		spin_unlock_irqrestore(&rtc_lock, flags);

		if ((val & RTC_UIP) == 0)
			return true;

		mdelay(1);
	}

	return false;
}
EXPORT_SYMBOL_GPL(mc146818_does_rtc_work);

int mc146818_get_time(struct rtc_time *time)
{
	unsigned char ctrl;
	unsigned long flags;
	unsigned int iter_count = 0;
	unsigned char century = 0;
	bool retry;
>>>>>>> upstream/android-13

#ifdef CONFIG_MACH_DECSTATION
	unsigned int real_year;
#endif

<<<<<<< HEAD
	/*
	 * read RTC once any update in progress is done. The update
	 * can take just over 2ms. We wait 20ms. There is no need to
	 * to poll-wait (up to 1s - eeccch) for the falling edge of RTC_UIP.
	 * If you need to know *exactly* when a second has started, enable
	 * periodic update complete interrupts, (via ioctl) and then
	 * immediately read /dev/rtc which will block until you get the IRQ.
	 * Once the read clears, read the RTC time (again via ioctl). Easy.
	 */
	if (mc146818_is_updating())
		mdelay(20);
=======
again:
	if (iter_count > 10) {
		memset(time, 0, sizeof(*time));
		return -EIO;
	}
	iter_count++;

	spin_lock_irqsave(&rtc_lock, flags);

	/*
	 * Check whether there is an update in progress during which the
	 * readout is unspecified. The maximum update time is ~2ms. Poll
	 * every msec for completion.
	 *
	 * Store the second value before checking UIP so a long lasting NMI
	 * which happens to hit after the UIP check cannot make an update
	 * cycle invisible.
	 */
	time->tm_sec = CMOS_READ(RTC_SECONDS);

	if (CMOS_READ(RTC_FREQ_SELECT) & RTC_UIP) {
		spin_unlock_irqrestore(&rtc_lock, flags);
		mdelay(1);
		goto again;
	}

	/* Revalidate the above readout */
	if (time->tm_sec != CMOS_READ(RTC_SECONDS)) {
		spin_unlock_irqrestore(&rtc_lock, flags);
		goto again;
	}
>>>>>>> upstream/android-13

	/*
	 * Only the values that we read from the RTC are set. We leave
	 * tm_wday, tm_yday and tm_isdst untouched. Even though the
	 * RTC has RTC_DAY_OF_WEEK, we ignore it, as it is only updated
	 * by the RTC when initially set to a non-zero value.
	 */
<<<<<<< HEAD
	spin_lock_irqsave(&rtc_lock, flags);
	time->tm_sec = CMOS_READ(RTC_SECONDS);
=======
>>>>>>> upstream/android-13
	time->tm_min = CMOS_READ(RTC_MINUTES);
	time->tm_hour = CMOS_READ(RTC_HOURS);
	time->tm_mday = CMOS_READ(RTC_DAY_OF_MONTH);
	time->tm_mon = CMOS_READ(RTC_MONTH);
	time->tm_year = CMOS_READ(RTC_YEAR);
#ifdef CONFIG_MACH_DECSTATION
	real_year = CMOS_READ(RTC_DEC_YEAR);
#endif
#ifdef CONFIG_ACPI
	if (acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID &&
	    acpi_gbl_FADT.century)
		century = CMOS_READ(acpi_gbl_FADT.century);
#endif
	ctrl = CMOS_READ(RTC_CONTROL);
<<<<<<< HEAD
	spin_unlock_irqrestore(&rtc_lock, flags);

=======
	/*
	 * Check for the UIP bit again. If it is set now then
	 * the above values may contain garbage.
	 */
	retry = CMOS_READ(RTC_FREQ_SELECT) & RTC_UIP;
	/*
	 * A NMI might have interrupted the above sequence so check whether
	 * the seconds value has changed which indicates that the NMI took
	 * longer than the UIP bit was set. Unlikely, but possible and
	 * there is also virt...
	 */
	retry |= time->tm_sec != CMOS_READ(RTC_SECONDS);

	spin_unlock_irqrestore(&rtc_lock, flags);

	if (retry)
		goto again;

>>>>>>> upstream/android-13
	if (!(ctrl & RTC_DM_BINARY) || RTC_ALWAYS_BCD)
	{
		time->tm_sec = bcd2bin(time->tm_sec);
		time->tm_min = bcd2bin(time->tm_min);
		time->tm_hour = bcd2bin(time->tm_hour);
		time->tm_mday = bcd2bin(time->tm_mday);
		time->tm_mon = bcd2bin(time->tm_mon);
		time->tm_year = bcd2bin(time->tm_year);
		century = bcd2bin(century);
	}

#ifdef CONFIG_MACH_DECSTATION
	time->tm_year += real_year - 72;
#endif

<<<<<<< HEAD
	if (century > 20)
=======
	if (century > 19)
>>>>>>> upstream/android-13
		time->tm_year += (century - 19) * 100;

	/*
	 * Account for differences between how the RTC uses the values
	 * and how they are defined in a struct rtc_time;
	 */
	if (time->tm_year <= 69)
		time->tm_year += 100;

	time->tm_mon--;

<<<<<<< HEAD
	return RTC_24H;
=======
	return 0;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mc146818_get_time);

/* Set the current date and time in the real time clock. */
int mc146818_set_time(struct rtc_time *time)
{
	unsigned long flags;
	unsigned char mon, day, hrs, min, sec;
	unsigned char save_control, save_freq_select;
	unsigned int yrs;
#ifdef CONFIG_MACH_DECSTATION
	unsigned int real_yrs, leap_yr;
#endif
	unsigned char century = 0;

	yrs = time->tm_year;
	mon = time->tm_mon + 1;   /* tm_mon starts at zero */
	day = time->tm_mday;
	hrs = time->tm_hour;
	min = time->tm_min;
	sec = time->tm_sec;

	if (yrs > 255)	/* They are unsigned */
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&rtc_lock, flags);
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_MACH_DECSTATION
	real_yrs = yrs;
	leap_yr = ((!((yrs + 1900) % 4) && ((yrs + 1900) % 100)) ||
			!((yrs + 1900) % 400));
	yrs = 72;

	/*
	 * We want to keep the year set to 73 until March
	 * for non-leap years, so that Feb, 29th is handled
	 * correctly.
	 */
	if (!leap_yr && mon < 3) {
		real_yrs--;
		yrs = 73;
	}
#endif

#ifdef CONFIG_ACPI
	if (acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID &&
	    acpi_gbl_FADT.century) {
		century = (yrs + 1900) / 100;
		yrs %= 100;
	}
#endif

	/* These limits and adjustments are independent of
	 * whether the chip is in binary mode or not.
	 */
<<<<<<< HEAD
	if (yrs > 169) {
		spin_unlock_irqrestore(&rtc_lock, flags);
		return -EINVAL;
	}
=======
	if (yrs > 169)
		return -EINVAL;
>>>>>>> upstream/android-13

	if (yrs >= 100)
		yrs -= 100;

<<<<<<< HEAD
	if (!(CMOS_READ(RTC_CONTROL) & RTC_DM_BINARY)
	    || RTC_ALWAYS_BCD) {
=======
	spin_lock_irqsave(&rtc_lock, flags);
	save_control = CMOS_READ(RTC_CONTROL);
	spin_unlock_irqrestore(&rtc_lock, flags);
	if (!(save_control & RTC_DM_BINARY) || RTC_ALWAYS_BCD) {
>>>>>>> upstream/android-13
		sec = bin2bcd(sec);
		min = bin2bcd(min);
		hrs = bin2bcd(hrs);
		day = bin2bcd(day);
		mon = bin2bcd(mon);
		yrs = bin2bcd(yrs);
		century = bin2bcd(century);
	}

<<<<<<< HEAD
=======
	spin_lock_irqsave(&rtc_lock, flags);
>>>>>>> upstream/android-13
	save_control = CMOS_READ(RTC_CONTROL);
	CMOS_WRITE((save_control|RTC_SET), RTC_CONTROL);
	save_freq_select = CMOS_READ(RTC_FREQ_SELECT);
	CMOS_WRITE((save_freq_select|RTC_DIV_RESET2), RTC_FREQ_SELECT);

#ifdef CONFIG_MACH_DECSTATION
	CMOS_WRITE(real_yrs, RTC_DEC_YEAR);
#endif
	CMOS_WRITE(yrs, RTC_YEAR);
	CMOS_WRITE(mon, RTC_MONTH);
	CMOS_WRITE(day, RTC_DAY_OF_MONTH);
	CMOS_WRITE(hrs, RTC_HOURS);
	CMOS_WRITE(min, RTC_MINUTES);
	CMOS_WRITE(sec, RTC_SECONDS);
#ifdef CONFIG_ACPI
	if (acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID &&
	    acpi_gbl_FADT.century)
		CMOS_WRITE(century, acpi_gbl_FADT.century);
#endif

	CMOS_WRITE(save_control, RTC_CONTROL);
	CMOS_WRITE(save_freq_select, RTC_FREQ_SELECT);

	spin_unlock_irqrestore(&rtc_lock, flags);

	return 0;
}
EXPORT_SYMBOL_GPL(mc146818_set_time);
