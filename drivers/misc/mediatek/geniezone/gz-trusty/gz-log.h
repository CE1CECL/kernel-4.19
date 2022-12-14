/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _GZ_LOG_H_
#define _GZ_LOG_H_

struct log_rb {
	uint32_t alloc;
	uint32_t put;
	uint32_t sz;
	char data[0];
} __packed;

#define TRUSTY_LOG_API_VERSION		(1)

void get_gz_log_buffer(unsigned long *addr, unsigned long *paddr,
		       unsigned long *size, unsigned long *start);
#endif
