/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MTK_SP_AFE_EXTERNAL_H_
#define _MTK_SP_AFE_EXTERNAL_H_

/* api for other modules to allocate audio sram */
int mtk_audio_request_sram(dma_addr_t *phys_addr,
			   unsigned char **virt_addr,
			   unsigned int length,
			   void *user);
void mtk_audio_free_sram(void *user);

/* conditional enter suspend */
bool mtk_audio_condition_enter_suspend(void);
#endif
