/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SSPM_SYSFS_H__
#define __SSPM_SYSFS_H__

#include <linux/sysfs.h>
#include <linux/device.h>

void sspm_log_if_wake(void);

extern int __init sspm_sysfs_init(void);
extern int sspm_sysfs_create_file(const struct device_attribute *attr);
#endif
