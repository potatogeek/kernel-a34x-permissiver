/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DT_BINDINGS_SAMSUNG_I2S_H
#define _DT_BINDINGS_SAMSUNG_I2S_H

<<<<<<< HEAD
#define CLK_I2S_CDCLK		0
#define CLK_I2S_RCLK_SRC	1
#define CLK_I2S_RCLK_PSR	2
=======
#define CLK_I2S_CDCLK		0 /* the CDCLK (CODECLKO) gate clock */

#define CLK_I2S_RCLK_SRC	1 /* the RCLKSRC mux clock (corresponding to
				   * RCLKSRC bit in IISMOD register)
				   */

#define CLK_I2S_RCLK_PSR	2 /* the RCLK prescaler divider clock
				   * (corresponding to the IISPSR register)
				   */
>>>>>>> upstream/android-13

#endif /* _DT_BINDINGS_SAMSUNG_I2S_H */
