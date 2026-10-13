/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

/**
 * @defgroup debug
 * @{
 *
 * @ingroup assert
 *
 * @brief Debug assert api's
 *
 * @version 1.0.0
 *
 * @date Oct 9 2026
 */

#ifndef __ASSERT__H
#define __ASSERT__H

#define LIKELY(expr) (__builtin_expect(!!(expr), 1))

#ifndef __assert_failed
#define __assert_failed() while (1)
#endif

#define __ASSERT(cond)                                                                             \
	do {                                                                                       \
		if (LIKELY((cond))) {                                                              \
			(void)0;                                                                   \
		} else {                                                                           \
			__assert_failed();                                                         \
		}                                                                                  \
	} while (0)

#endif // __ASSERT__H

/**@}*/
