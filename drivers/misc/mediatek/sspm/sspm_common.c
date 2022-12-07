// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>       /* needed by all modules */
#include <linux/ioport.h>
#include <linux/io.h>
#include "sspm_common.h"

struct sspm_regs sspmreg;
struct platform_device *sspm_pdev;

unsigned int sspm_ready;

void memcpy_to_sspm(void __iomem *trg, const void *src, int size)
{
	int i, len;
	u32 __iomem *t = trg;
	const u32 *s = src;

	len = (size + 3) >> 2;

	for (i = 0; i < len; i++)
		*t++ = *s++;
}

void memcpy_from_sspm(void *trg, const void __iomem *src, int size)
{
	int i, len;
	u32 *t = trg;
	const u32 __iomem *s = src;

	len = (size + 3) >> 2;

	for (i = 0; i < len; i++)
		*t++ = *s++;
}

unsigned int is_sspm_ready(void)
{
	return sspm_ready ? 1 : 0;
}
EXPORT_SYMBOL_GPL(is_sspm_ready);
