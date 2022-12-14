/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MT6779_DBG_FS_COMMON_H__
#define __MT6779_DBG_FS_COMMON_H__

/* Determine for user name handle */
#define MT_LP_RQ_USER_NAME_LEN	(4)
#define MT_LP_RQ_USER_CHAR_U	(8)
#define MT_LP_RQ_USER_CHAR_MASK	(0xFF)

/* Determine for resource usage id */
#define MT_LP_RQ_ID_ALL_USAGE	(-1)


int mt6779_dbg_idle_fs_init(void);

int mt6779_dbg_idle_fs_deinit(void);

int mt6779_dbg_spm_fs_init(void);

int mt6779_dbg_spm_fs_deinit(void);

int mt6779_logger_init(void);

int mt6779_dbg_fs_init(void);

void mt6779_dbg_fs_exit(void);

int mtk_cpupm_dbg_init(void);

void mtk_cpupm_dbg_exit(void);

#endif /* __MTK_LP_PLAT_FS_H__ */
