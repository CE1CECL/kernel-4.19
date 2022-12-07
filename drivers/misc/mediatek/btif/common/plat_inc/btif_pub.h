/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __HAL_BTIF_PUB_H_
#define __HAL_BTIF_PUB_H_

#include "plat_common.h"

/*Enum Defination*/
/*BTIF Mode Enum */
enum _ENUM_BTIF_MODE_ {
	BTIF_MODE_PIO = 0,
	BTIF_MODE_DMA = BTIF_MODE_PIO + 1,
	BTIF_MODE_MAX,
};

struct _MTK_BTIF_INFO_STR_ *hal_btif_info_get(void);

#if !defined(CONFIG_MTK_CLKMGR)
int hal_btif_clk_get_and_prepare(struct platform_device *pdev);
int hal_btif_clk_unprepare(void);
#endif
int hal_btif_clk_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
		      enum _ENUM_CLOCK_CTRL_ flag);

int hal_btif_hw_init(struct _MTK_BTIF_INFO_STR_ *p_btif);

int hal_btif_rx_cb_reg(struct _MTK_BTIF_INFO_STR_ *p_btif_info,
		       btif_rx_buf_write rx_cb);

int hal_btif_loopback_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif, bool en);

int hal_btif_irq_handler(struct _MTK_BTIF_INFO_STR_ *p_btif,
			 unsigned char *p_buf, const unsigned int max_len);

int hal_btif_tx_mode_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
			  enum _ENUM_BTIF_MODE_ mode);

int hal_btif_rx_mode_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
			  enum _ENUM_BTIF_MODE_ mode);

int hal_btif_send_data(struct _MTK_BTIF_INFO_STR_ *p_btif,
		       const unsigned char *p_buf, const unsigned int buf_len);

int hal_btif_raise_wak_sig(struct _MTK_BTIF_INFO_STR_ *p_btif);

int hal_btif_dump_reg(struct _MTK_BTIF_INFO_STR_ *p_btif,
		      enum _ENUM_BTIF_REG_ID_ flag);

bool hal_btif_is_tx_complete(struct _MTK_BTIF_INFO_STR_ *p_btif);

bool hal_btif_is_tx_allow(struct _MTK_BTIF_INFO_STR_ *p_btif);

int hal_btif_pm_ops(struct _MTK_BTIF_INFO_STR_ *p_btif,
		    enum _MTK_BTIF_PM_OPID_ opid);

void mtk_btif_read_cpu_sw_rst_debug_plat(void);

#endif /*__HAL_BTIF_PUB_H_*/
