/* SPDX-License-Identifier: GPL-2.0 */

#if !defined(__MTK_PRINTK_CTRL_H__)
#define __MTK_PRINTK_CTRL_H__

#include <linux/kernel.h>
#include <linux/sched.h>

#ifdef CONFIG_PRINTK_MTK_UART_CONSOLE
bool mt_get_uartlog_status(void);
void mt_disable_uart(void);
void mt_enable_uart(void);
#else
static inline bool mt_get_uartlog_status(void)
{
	return false;
}
static inline void mt_disable_uart(void)
{

}
static inline void mt_enable_uart(void)
{

}
#endif

#endif				/* __MTK_PRINTK_CTRL_H__ */
