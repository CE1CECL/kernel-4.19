/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MT6797_AFE_CLK_H_
#define _MT6797_AFE_CLK_H_

struct mtk_base_afe;

int mt6797_init_clock(struct mtk_base_afe *afe);
int mt6797_afe_enable_clock(struct mtk_base_afe *afe);
int mt6797_afe_disable_clock(struct mtk_base_afe *afe);
#endif
