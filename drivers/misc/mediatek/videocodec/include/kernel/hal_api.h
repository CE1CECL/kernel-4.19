/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _HAL_API_H_
#define _HAL_API_H_
#include "val_types_public.h"
#include "hal_types_private.h"

#ifdef __cplusplus
extern "C" {
#endif


VAL_RESULT_T eHalInit(unsigned long *a_phHalHandle,
			HAL_CODEC_TYPE_T a_eHalCodecType);


VAL_RESULT_T eHalDeInit(unsigned long *a_phHalHandle);


unsigned long eHalGetMMAP(unsigned long *a_hHalHandle, unsigned int RegAddr);


VAL_RESULT_T eHalCmdProc(
	unsigned long *a_hHalHandle,
	HAL_CMD_T a_eHalCmd,
	void *a_pvInParam,
	void *a_pvOutParam
);



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _HAL_API_H_ */
