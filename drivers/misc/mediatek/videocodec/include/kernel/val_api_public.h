/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VAL_API_PUBLIC_H_
#define _VAL_API_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "val_types_public.h"


unsigned int eVideoInitMVA(void **a_pvHandle);


unsigned int eVideoAllocMVA(
	void *a_pvHandle,
	unsigned int a_u4Va,
	unsigned int *ap_u4Pa,
	unsigned int a_u4Size,
	struct VAL_VCODEC_M4U_BUFFER_CONFIG_T *a_pvM4uConfig
);

unsigned int eVideoFreeMVA(
	void *a_pvHandle,
	unsigned int a_u4Va,
	unsigned int a_u4Pa,
	unsigned int a_u4Size,
	struct VAL_VCODEC_M4U_BUFFER_CONFIG_T *a_pvM4uConfig
);


unsigned int eVideoDeInitMVA(void *a_pvHandle);


int eVideoGetM4UModuleID(unsigned int u4MemType);


enum VAL_RESULT_T eVideoAtoi(struct VAL_ATOI_T *a_prParam,
		unsigned int a_u4ParamSize);


enum VAL_RESULT_T eVideoStrStr(struct VAL_STRSTR_T *a_prParam,
				unsigned int a_u4ParamSize);


enum VAL_RESULT_T eVideoFlushCache(struct VAL_MEMORY_T *a_prParam,
			unsigned int a_u4ParamSize, unsigned int optype);


enum VAL_RESULT_T eVideoInvalidateCache
			(struct VAL_MEMORY_T *a_prParam,
			unsigned int a_u4ParamSize, unsigned int optype);


enum VAL_RESULT_T eVideoMemAlloc(struct VAL_MEMORY_T *a_prParam,
			unsigned int a_u4ParamSize);


enum VAL_RESULT_T eVideoMemFree(struct VAL_MEMORY_T *a_prParam,
				unsigned int a_u4ParamSize);


enum VAL_RESULT_T eVideoMemSet(
	struct VAL_MEMORY_T *a_prParam,
	unsigned int a_u4ParamSize,
	int a_u4Value,
	unsigned int a_u4Size
);


enum VAL_RESULT_T eVideoMemCpy(
	struct VAL_MEMORY_T *a_prParamDst,
	unsigned int a_u4ParamDstSize,
	struct VAL_MEMORY_T *a_prParamSrc,
	unsigned int a_u4ParamSrcSize,
	unsigned int a_u4Size
);


enum VAL_RESULT_T eVideoMemCmp(
	struct VAL_MEMORY_T *a_prParamSrc1,
	unsigned int a_u4ParamSrc1Size,
	struct VAL_MEMORY_T *a_prParamSrc2,
	unsigned int a_u4ParamSrc2Size,
	unsigned int a_u4Size
);


enum VAL_RESULT_T WaitISR(struct VAL_ISR_T *a_prParam,
				unsigned int a_u4ParamSize);


enum VAL_RESULT_T eVideoLockHW(struct VAL_HW_LOCK_T *a_prParam,
			unsigned int  a_u4ParamSize);


enum VAL_RESULT_T eVideoUnLockHW(struct VAL_HW_LOCK_T *a_prParam,
			unsigned int  a_u4ParamSize);


enum VAL_RESULT_T eVideoGetTimeOfDay(struct VAL_TIME_T *a_prParam,
				unsigned int a_u4ParamSize);


enum VAL_RESULT_T eHalEMICtrlForRecordSize
			(struct VAL_RECORD_SIZE_T *a_prDrvRecordSize);


enum VAL_RESULT_T eVideoVcodecSetThreadID
			(struct VAL_VCODEC_THREAD_ID_T *a_prThreadID);


enum VAL_RESULT_T eVideoGetParam(enum VAL_GET_TYPE_T a_eType,
			void *a_pvInParam, void *a_pvOutParam);

enum VAL_RESULT_T eVideoSetParam(enum VAL_SET_TYPE_T a_eType,
			void *a_pvInParam, void *a_pvOutParam);

enum VAL_RESULT_T eVideoE3TCMPowerON(unsigned int a_u4E3TCMClk);
enum VAL_RESULT_T eVideoE3TCMPowerOFF(unsigned int a_u4E3TCMClk);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _VAL_API_PUBLIC_H_ */
