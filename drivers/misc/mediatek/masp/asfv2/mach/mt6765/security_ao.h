/* SPDX-License-Identifier: GPL-2.0 */

#ifndef SECURITY_AO_H
#define SECURITY_AO_H

extern void __iomem *security_ao_base;


/* miscx index*/
#define BOOT_MISC2_IDX (2)

#define RST_CON_BIT(idx) (0x1 << idx)

/* boot misc2 flags */
#define BOOT_MISC2_VERITY_ERR (0x1 << 0)

#define MISC_LOCK_KEY_MAGIC    (0x0000ad98)

#define BOOT_MISC2       (security_ao_base + 0x088)
#define MISC_LOCK_KEY    (security_ao_base + 0x100)
#define RST_CON          (security_ao_base + 0x108)

#endif /* SECURITY_AO_H */
