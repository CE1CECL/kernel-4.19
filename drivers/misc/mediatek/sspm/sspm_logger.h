/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SSPM_LOGGER_H__
#define __SSPM_LOGGER_H__

#include <linux/types.h>

extern unsigned int __init sspm_logger_init(phys_addr_t start,
	phys_addr_t limit);
extern int __init sspm_logger_init_done(void);
#endif
