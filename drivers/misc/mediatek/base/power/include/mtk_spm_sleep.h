/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MT_SPM_SLEEP_H__
#define __MT_SPM_SLEEP_H__

#if defined(CONFIG_MACH_MT6757) || defined(CONFIG_MACH_KIBOPLUS)

#include "spm_v2/mtk_spm_sleep.h"

#elif defined(CONFIG_MACH_MT6763)

#include "spm_v4/mtk_spm_sleep.h"

#else

#include "spm/mtk_spm_sleep.h"

#endif

#endif /* __MT_SPM_SLEEP_H__ */

