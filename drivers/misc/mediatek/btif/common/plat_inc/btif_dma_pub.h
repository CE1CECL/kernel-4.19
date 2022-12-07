/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __HAL_BTIFD_DMA_PUB_H_
#define __HAL_BTIFD_DMA_PUB_H_

#include <linux/dma-mapping.h>

#include "plat_common.h"

enum _ENUM_DMA_CTRL_ {
	DMA_CTRL_DISABLE = 0,
	DMA_CTRL_ENABLE = DMA_CTRL_DISABLE + 1,
	DMA_CTRL_BOTH,
};

struct _MTK_DMA_INFO_STR_ *hal_btif_dma_info_get(enum _ENUM_DMA_DIR_ dma_dir);

int hal_btif_dma_hw_init(struct _MTK_DMA_INFO_STR_ *p_dma_info);

int hal_btif_dma_clk_ctrl(struct _MTK_DMA_INFO_STR_ *p_dma_info,
			  enum _ENUM_CLOCK_CTRL_ flag);

int hal_btif_dma_ctrl(struct _MTK_DMA_INFO_STR_ *p_dma_info,
		      enum _ENUM_DMA_CTRL_  ctrl_id);

int hal_btif_dma_rx_cb_reg(struct _MTK_DMA_INFO_STR_ *p_dma_info,
			   dma_rx_buf_write rx_cb);

int hal_btif_vfifo_reset(struct _MTK_DMA_INFO_STR_ *p_dma_info);

int hal_tx_dma_irq_handler(struct _MTK_DMA_INFO_STR_ *p_dma_info);

int hal_dma_send_data(struct _MTK_DMA_INFO_STR_ *p_dma_info,
		      const unsigned char *p_buf, const unsigned int buf_len);

bool hal_dma_is_tx_complete(struct _MTK_DMA_INFO_STR_ *p_dma_info);

int hal_dma_get_ava_room(struct _MTK_DMA_INFO_STR_ *p_dma_info);

bool hal_dma_is_tx_allow(struct _MTK_DMA_INFO_STR_ *p_dma_info);

int hal_rx_dma_irq_handler(struct _MTK_DMA_INFO_STR_ *p_dma_info,
			   unsigned char *p_buf, const unsigned int max_len);

int hal_dma_dump_reg(struct _MTK_DMA_INFO_STR_ *p_dma_info,
		     enum _ENUM_BTIF_REG_ID_ flag);

int hal_dma_pm_ops(struct _MTK_DMA_INFO_STR_ *p_dma_info,
		   enum _MTK_BTIF_PM_OPID_ opid);

int hal_dma_tx_has_pending(struct _MTK_DMA_INFO_STR_ *p_dma_info);
int hal_dma_rx_has_pending(struct _MTK_DMA_INFO_STR_ *p_dma_info);
int hal_rx_dma_lock(bool enable);

#endif /*__HAL_BTIFD_DMA_PUB_H_*/
