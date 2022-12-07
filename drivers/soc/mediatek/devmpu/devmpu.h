/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __DEVMPU_H__
#define __DEVMPU_H__

void devmpu_vio_clear(unsigned int emi_id);

int devmpu_print_violation(uint64_t vio_addr, uint32_t vio_id,
		uint32_t vio_domain, uint32_t vio_rw, bool from_emimpu);

#endif /* __DEVMPU_H__ */
