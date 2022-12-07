/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MTK_SCP_H
#define _MTK_SCP_H

#include <linux/platform_device.h>

typedef void (*scp_ipi_handler_t) (void *data,
				   unsigned int len,
				   void *priv);


enum scp_ipi_id {
	SCP_IPI_INIT = 0,
	SCP_IPI_VDEC_H264,
	SCP_IPI_VDEC_VP8,
	SCP_IPI_VDEC_VP9,
	SCP_IPI_VENC_H264,
	SCP_IPI_VENC_VP8,
	SCP_IPI_MDP,
	SCP_IPI_CROS_HOST_CMD,
	SCP_IPI_NS_SERVICE = 0xFF,
	SCP_IPI_MAX = 0x100,
};


int scp_ipi_register(struct platform_device *pdev,
		     enum scp_ipi_id id,
		     scp_ipi_handler_t handler,
		     void *priv);

void scp_ipi_unregister(struct platform_device *pdev, enum scp_ipi_id id);

int scp_ipi_send(struct platform_device *pdev,
		 enum scp_ipi_id id,
		 void *buf,
		 unsigned int len,
		 unsigned int wait);

struct platform_device *scp_get_pdev(struct platform_device *pdev);

unsigned int scp_get_vdec_hw_capa(struct platform_device *pdev);

unsigned int scp_get_venc_hw_capa(struct platform_device *pdev);

void *scp_mapping_dm_addr(struct platform_device *pdev,
			  u32 mem_addr);

#define SCP_RESERVED_MEM	(1)
#if SCP_RESERVED_MEM
/* scp reserve memory ID definition*/
enum scp_reserve_mem_id_t {
	SCP_ISP_MEM_ID,
	SCP_NUMS_MEM_ID,
};

struct scp_reserve_mblock {
	enum scp_reserve_mem_id_t num;
	u64 start_phys;
	u64 start_virt;
	u64 size;
};

extern phys_addr_t scp_get_reserve_mem_phys(enum scp_reserve_mem_id_t id);
extern phys_addr_t scp_get_reserve_mem_virt(enum scp_reserve_mem_id_t id);
extern phys_addr_t scp_get_reserve_mem_size(enum scp_reserve_mem_id_t id);
#endif

#endif /* _MTK_SCP_H */
