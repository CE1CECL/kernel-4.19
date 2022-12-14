/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MTK_CM_MGR_PLATFORM_H__
#define __MTK_CM_MGR_PLATFORM_H__

#define PERF_TIME 3000

#include "mtk_cm_mgr_reg_mt6779.h"

#define CREATE_TRACE_POINTS
#include "mtk_cm_mgr_events_mt6779.h"

#ifdef CONFIG_MTK_DRAMC_LEGACY
#include <mtk_dramc.h>
#endif /* CONFIG_MTK_DRAMC_LEGACY */

#define CM_MGR_CPU_CLUSTER 2

enum {
	CM_MGR_LP4X_2CH = 0,
	CM_MGR_MAX,
};

#endif	/* __MTK_CM_MGR_PLATFORM_H__ */
