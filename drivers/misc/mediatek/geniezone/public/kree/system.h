/* SPDX-License-Identifier: GPL-2.0 */





#ifndef __KREE_SYSTEM_H__
#define __KREE_SYSTEM_H__

#if IS_ENABLED(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT)	\
	|| IS_ENABLED(CONFIG_MTK_ENABLE_GENIEZONE)

#include <tz_cross/trustzone.h>
#include <gz-trusty/trusty.h>

void KREE_SESSION_LOCK(int32_t handle);
void KREE_SESSION_UNLOCK(int32_t handle);

int gz_get_cpuinfo_thread(void *data);
void set_gz_bind_cpu(int on);
int get_gz_bind_cpu(void);
int ree_dummy_thread(void *data);

struct _cpus_cluster_freq {
	unsigned int max_freq;
	unsigned int min_freq;
};

#include "mem.h"

#if IS_ENABLED(CONFIG_GZ_VPU_WITH_M4U)
int gz_do_m4u_map(KREE_SHAREDMEM_HANDLE handle,
					phys_addr_t pa, uint32_t size,
					uint32_t region_id);
int gz_do_m4u_umap(KREE_SHAREDMEM_HANDLE handle);
#endif


/* Session Management */
TZ_RESULT KREE_CreateSession(const char *ta_uuid, KREE_SESSION_HANDLE *pHandle);


/*fix mtee sync*/
TZ_RESULT KREE_CreateSessionWithTag(const char *ta_uuid,
				    KREE_SESSION_HANDLE *pHandle,
				    const char *tag);

TZ_RESULT KREE_CloseSession(KREE_SESSION_HANDLE handle);


TZ_RESULT KREE_TeeServiceCall(KREE_SESSION_HANDLE handle, uint32_t command,
			      uint32_t paramTypes, union MTEEC_PARAM param[4]);


/* System Hardware Counter */
u64 KREE_GetSystemCnt(void);


u32 KREE_GetSystemCntFrq(void);

TZ_RESULT KREE_SessionToTID(KREE_SESSION_HANDLE session, enum tee_id_t *o_tid);

#endif /* CONFIG_MTK_IN_HOUSE_TEE_SUPPORT || CONFIG_MTK_ENABLE_GENIEZONE */
#endif /* __KREE_H__ */
