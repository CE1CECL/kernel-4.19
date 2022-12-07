
#ifndef _MTK_VPU_H
#define _MTK_VPU_H

#include <linux/platform_device.h>


typedef int (*ipi_handler_t) (void *data,
			       unsigned int len,
			       void *priv);


enum ipi_id {
	IPI_VPU_INIT = 0,
	IPI_VDEC_H264,
	IPI_VDEC_VP8,
	IPI_VDEC_VP9,
	IPI_VENC_H264,
	IPI_VENC_VP8,
	IPI_MDP,
	IPI_MAX,
};

enum rst_id {
	VPU_RST_ENC,
	VPU_RST_DEC,
	VPU_RST_MDP,
	VPU_RST_MAX,
};

int vpu_ipi_register(struct platform_device *pdev, enum ipi_id id,
		     ipi_handler_t handler, const char *name, void *priv);

int vpu_ipi_send(struct platform_device *pdev,
		 enum ipi_id id, void *buf,
		 unsigned int len);

struct platform_device *vpu_get_plat_device(struct platform_device *pdev);

int vpu_wdt_reg_handler(struct platform_device *pdev,
			void vpu_wdt_reset_func(void *),
			void *priv, enum rst_id id);

unsigned int vpu_get_vdec_hw_capa(struct platform_device *pdev);

unsigned int vpu_get_venc_hw_capa(struct platform_device *pdev);

int vpu_load_firmware(struct platform_device *pdev);

void *vpu_mapping_dm_addr(struct platform_device *pdev,
			  u32 dtcm_dmem_addr);
#endif /* _MTK_VPU_H */
