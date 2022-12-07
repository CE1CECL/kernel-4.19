/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MTK_SPM_IRQ_EDGE_H__
#define __MTK_SPM_IRQ_EDGE_H__


struct edge_trigger_irq_list {
	const char *name;
	int order;
	unsigned int wakesrc;
};

static struct edge_trigger_irq_list list[] = {
	{ "mediatek,mt6761-infracfg",   0,	0 },
	{ "mediatek,kp",            0,  WAKE_SRC_R12_KP_IRQ_B },
	{ "mediatek,mddriver",       3,  WAKE_SRC_R12_MD1_WDT_B },
};

#endif /* __MTK_SPM_IRQ_EDGE_H__ */
