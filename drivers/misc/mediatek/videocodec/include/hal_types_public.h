/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _HAL_TYPES_PUBLIC_H_
#define _HAL_TYPES_PUBLIC_H_

#include "val_types_public.h"

struct _HAL_POWER_T {
	void *pvHandle;
	unsigned int u4HandleSize;
	enum VAL_DRIVER_TYPE_T eDriverType;
	char fgEnable;
	void *pvReserved;
	unsigned int u4ReservedSize;
};

struct HAL_ISR_T {
	void *pvHandle;
	unsigned int u4HandleSize;
	enum VAL_DRIVER_TYPE_T eDriverType;
	char fgRegister;
	void *pvReserved;
	unsigned int u4ReservedSize;
};

#endif /* #ifndef _HAL_TYPES_PUBLIC_H_ */
