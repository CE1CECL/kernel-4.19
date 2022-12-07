// SPDX-License-Identifier: GPL-2.0

#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <mt-plat/sync_write.h>
#ifdef CONFIG_MTK_AEE_FEATURE
#include <mt-plat/aee.h>
#endif

#include <emi_io.h>
#include "mt_emi.h"

void plat_debug_api_init(void)
{
}

void dump_emi_outstanding(void)
{
}
