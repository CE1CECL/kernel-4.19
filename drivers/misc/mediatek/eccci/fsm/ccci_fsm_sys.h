/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __CCCI_FSM_SYS_H__
#define __CCCI_FSM_SYS_H__

#define AED_STR_LEN		(2048)

struct mdee_info_collect {
	spinlock_t mdee_info_lock;
	char mdee_info[AED_STR_LEN];
};

void fsm_sys_mdee_info_notify(char *buf);
int fsm_sys_init(void);

#endif /* __CCCI_FSM_SYS_H__ */
