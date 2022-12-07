/* SPDX-License-Identifier: GPL-2.0 */

#ifndef MTEE_REGIONS_H
#define MTEE_REGIONS_H

enum MTEE_MCHUNKS_ID {
	MTEE_MCHUNKS_PROT = 0,
	MTEE_MCHUNKS_HAPP = 1,
	MTEE_MCHUNKS_HAPP_EXTRA = 2,
	MTEE_MCHUNKS_SDSP = 3,
	MTEE_MCHUNKS_SDSP_SHARED_VPU_TEE = 4,
	MTEE_MCHUNKS_SDSP_SHARED_MTEE_TEE = 5,
	MTEE_MCHUNKS_SDSP_SHARED_VPU_MTEE_TEE = 6,

	MTEE_MCUHNKS_INVALID = 0xFFFFFFFF,
};

#endif /* end of MTEE_REGIONS_H */
