/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SENINF_H__
#define __SENINF_H__

#include "seninf_cfg.h"
#include "seninf_clk.h"

#include <linux/atomic.h>
#include <linux/mutex.h>

#define SENINF_DEV_NAME "seninf"

#ifdef DFS_CTRL_BY_OPP
#include <linux/pm_opp.h>
#include <linux/regulator/consumer.h>
struct seninf_dfs_ctx {
	struct device *dev;
	struct regulator *reg;
	unsigned long *freqs;
	unsigned long *volts;
	int cnt;
};
#endif

struct SENINF {
	dev_t dev_no;
	struct cdev *pchar_dev;
	struct class *pclass;
	struct device *dev;

	struct SENINF_CLK clk;

	void __iomem *pseninf_base[SENINF_MAX_NUM];

	struct mutex seninf_mutex;
	atomic_t seninf_open_cnt;

#ifdef DFS_CTRL_BY_OPP
	struct seninf_dfs_ctx dfs_ctx;
#endif
};
extern MINT32 seninf_dump_reg(void);

#endif

