/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MTK_LPM_DATA_TYPE_H__
#define __MTK_LPM_DATA_TYPE_H__

struct mtk_lpm_data {
	union {
		int i32;
		unsigned int u32;
		void *p;
	} d;
};

#endif
