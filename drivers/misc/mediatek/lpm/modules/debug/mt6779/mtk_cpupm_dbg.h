/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MTK_CPUPM_DBG_H__
#define __MTK_CPUPM_DBG_H__

int mtk_cpupm_mcusys_write(int ofs, unsigned int val);
unsigned int mtk_cpupm_mcusys_read(int ofs);
int mtk_cpupm_syssram_write(int ofs, unsigned int val);
unsigned int mtk_cpupm_syssram_read(int ofs);

void mtk_cpupm_block(void);
void mtk_cpupm_allow(void);

int mtk_cpupm_get_idle_state_count(int cpu);
#endif /* __MTK_CPUPM_DBG_H__ */
