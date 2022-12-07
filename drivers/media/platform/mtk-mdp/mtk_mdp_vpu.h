
#ifndef __MTK_MDP_VPU_H__
#define __MTK_MDP_VPU_H__

#include "mtk_mdp_ipi.h"


struct mtk_mdp_vpu {
	struct platform_device	*pdev;
	uint32_t		inst_addr;
	int32_t			failure;
	struct mdp_process_vsi	*vsi;
};

int mtk_mdp_vpu_register(struct platform_device *pdev);
int mtk_mdp_vpu_init(struct mtk_mdp_vpu *vpu);
int mtk_mdp_vpu_deinit(struct mtk_mdp_vpu *vpu);
int mtk_mdp_vpu_process(struct mtk_mdp_vpu *vpu);

#endif /* __MTK_MDP_VPU_H__ */
