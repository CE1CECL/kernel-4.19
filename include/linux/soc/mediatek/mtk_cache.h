/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MTK_CACHE__
#define __MTK_CACHE__

void __inner_flush_dcache_L1(void);
void __inner_flush_dcache_L2(void);
void __inner_flush_dcache_all(void);
void __inner_clean_dcache_L1(void);
void __inner_clean_dcache_L2(void);
void __inner_clean_dcache_all(void);
void __inner_inv_dcache_L1(void);
void __inner_inv_dcache_L2(void);
void __inner_inv_dcache_all(void);
void __disable_dcache__inner_flush_dcache_L1(void);
void __disable_dcache__inner_flush_dcache_L1__inner_flush_dcache_L2(void);
void __disable_dcache__inner_clean_dcache_L1__inner_clean_dcache_L2(void);
void dis_D_inner_fL1L2(void);
void dis_D_inner_flush_all(void);
void __flush_dcache_user_area(void *start, unsigned int size);
void __clean_dcache_user_area(void *start, unsigned int size);
void __inval_dcache_user_area(void *start, unsigned int size);

#endif /* end of __MTK_CACHE__ */
