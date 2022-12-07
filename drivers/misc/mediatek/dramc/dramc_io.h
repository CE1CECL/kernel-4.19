/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRAMC_IO_H__
#define __DRAMC_IO_H__

/* only for arm64 */
#ifdef CONFIG_ARM64
#define IOMEM(a)	((void __force __iomem *)((a)))
#endif

#endif  /* __DRAMC_IO_H__ */

