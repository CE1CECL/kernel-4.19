/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __PLAT_SRAM_FLAG_H__
#define __PLAT_SRAM_FLAG_H__

/* each flag is a word */
struct plat_sram_flag {
	unsigned int plat_magic;
	unsigned int plat_sram_flag0;
	unsigned int plat_sram_flag1;
	unsigned int plat_sram_flag2;
};

#define DEF_PLAT_SRAM_FLAG PLAT_SRAM_FLAG_T

#define PLAT_SRAM_FLAG_KEY	0xDB45
#define OFFSET_LASTPC_VALID			0
#define OFFSET_LASTPC_VALID_BEFORE_REBOOT	1
#define OFFSET_ETB_0				2

#define MAX_ETB_NUM		10
/* available user type is 0x0~0x7 */
#define MAX_ETB_USER_NUM	8


#define OFFSET_DFD_VALID                     0
#define OFFSET_DFD_VALID_BEFORE_REBOOT       1



int set_sram_flag_lastpc_valid(void);
int set_sram_flag_dfd_valid(void);
int set_sram_flag_etb_user(unsigned int utb_id, unsigned int user_id);

extern void __iomem *get_dbg_info_base(unsigned int key);
extern unsigned int get_dbg_info_size(unsigned int key);

#endif
