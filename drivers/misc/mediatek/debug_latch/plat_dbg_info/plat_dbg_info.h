/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __PLAT_DBG_INFO_H__
#define __PLAT_DBG_INFO_H__

void __iomem *get_dbg_info_base(unsigned int key);
unsigned int get_dbg_info_size(unsigned int key);

#endif
