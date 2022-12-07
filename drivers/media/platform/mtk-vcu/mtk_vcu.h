/* SPDX-License-Identifier: GPL-2.0 */

#ifndef MTK_VCU_H
#define MTK_VCU_H

#include <aee.h>
#include <linux/fdtable.h>
#include <linux/platform_device.h>

#ifdef CONFIG_MTK_AEE_FEATURE
#define vcu_aee_print(string, args...) do {\
	char vcu_name[100];\
	int ret;\
	ret = snprintf(vcu_name, 100, "[VCU] "string, ##args); \
	if (ret > 0)\
		aee_kernel_warning_api(__FILE__, __LINE__, \
			DB_OPT_MMPROFILE_BUFFER | DB_OPT_NE_JBT_TRACES, \
			vcu_name, "[VCU] error:"string, ##args); \
	pr_info("[VCU] error:"string, ##args);  \
	} while (0)
#else
#define vcu_aee_print(string, args...) do {\
		pr_info("[VCU] error:"string, ##args);  \
	} while (0)

#endif


typedef int (*ipi_handler_t)(void *data,
							 unsigned int len,
							 void *priv);


enum ipi_id {
	IPI_VCU_INIT = 0,
	IPI_VDEC_COMMON,
	IPI_VDEC_H264,
	IPI_VDEC_H265,
	IPI_VDEC_HEIF,
	IPI_VDEC_VP8,
	IPI_VDEC_VP9,
	IPI_VDEC_MPEG4,
	IPI_VDEC_H263,
	IPI_VDEC_S263,
	IPI_VDEC_XVID,
	IPI_VDEC_MPEG12,
	IPI_VDEC_WMV,
	IPI_VDEC_RV30,
	IPI_VDEC_RV40,
	IPI_VDEC_AV1,
	IPI_VENC_COMMON,
	IPI_VENC_H264,
	IPI_VENC_H265,
	IPI_VENC_HEIF,
	IPI_VENC_VP8,
	IPI_VENC_MPEG4,
	IPI_VENC_HYBRID_H264,
	IPI_VENC_H263,
	IPI_MDP,
	IPI_MDP_1,
	IPI_MDP_2,
	IPI_MDP_3,
	IPI_CAMERA,
	IPI_MAX = 50,
};

enum vcu_codec_type {
	VCU_VDEC = 0,
	VCU_VENC,
	VCU_CODEC_MAX
};

int vcu_ipi_register(struct platform_device *pdev, enum ipi_id id,
	ipi_handler_t handler, const char *name, void *priv);

int vcu_ipi_send(struct platform_device *pdev,
				 enum ipi_id id, void *buf,
				 unsigned int len, void *priv);

struct platform_device *vcu_get_plat_device(struct platform_device *pdev);

unsigned int vcu_get_vdec_hw_capa(struct platform_device *pdev);

unsigned int vcu_get_venc_hw_capa(struct platform_device *pdev);

int vcu_load_firmware(struct platform_device *pdev);

int vcu_compare_version(struct platform_device *pdev,
						const char *expected_version);

void *vcu_mapping_dm_addr(struct platform_device *pdev,
						  uintptr_t dtcm_dmem_addr);

void vcu_get_task(struct task_struct **task, struct files_struct **f,
		int reset);
void vcu_get_file_lock(void);
void vcu_put_file_lock(void);
int vcu_get_sig_lock(unsigned long *flags);
void vcu_put_sig_lock(unsigned long flags);
int vcu_check_vpud_alive(void);
extern void smp_inner_dcache_flush_all(void);
int vcu_get_ctx_ipi_binding_lock(struct platform_device *pdev,
	struct mutex **mutex, unsigned long type);
int vcu_set_codec_ctx(struct platform_device *pdev,
		 void *codec_ctx, struct vb2_buffer *src_vb,
		 struct vb2_buffer *dst_vb, unsigned long type);
int vcu_clear_codec_ctx(struct platform_device *pdev,
		 void *codec_ctx, unsigned long type);
extern void venc_encode_prepare(void *ctx_prepare,
		unsigned int core_id, unsigned long *flags);
extern void venc_encode_unprepare(void *ctx_prepare,
		unsigned int core_id, unsigned long *flags);
extern void venc_encode_pmqos_gce_begin(void *ctx_begin,
		unsigned int core_id, int job_cnt);
extern void venc_encode_pmqos_gce_end(void *ctx_end,
		unsigned int core_id, int job_cnt);
extern void mtk_vcodec_gce_timeout_dump(void *ctx);
#endif /* _MTK_VCU_H */
