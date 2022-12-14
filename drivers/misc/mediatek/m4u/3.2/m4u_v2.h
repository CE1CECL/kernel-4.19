/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __M4U_V2_H__
#define __M4U_V2_H__
#include <linux/ioctl.h>
#include <linux/fs.h>
#include "m4u_port.h"
#include <linux/scatterlist.h>
#include "m4u_v2_ext.h"

struct m4u_port_array {
	#define M4U_PORT_ATTR_EN		(1<<0)
	#define M4U_PORT_ATTR_VIRTUAL	(1<<1)
	#define M4U_PORT_ATTR_SEC		(1<<2)
	unsigned char ports[M4U_PORT_NR];
};

int m4u_dump_info(int m4u_index);
int m4u_power_on(int m4u_index);
int m4u_power_off(int m4u_index);

int m4u_alloc_mva(struct m4u_client_t *client,
					int port,
					unsigned long va,
					struct sg_table *sg_table,
					unsigned int size, unsigned int prot,
					unsigned int flags,
					unsigned int *pMva);

int m4u_dealloc_mva(struct m4u_client_t *client,
			int port, unsigned int mva);

int m4u_config_port(struct M4U_PORT_STRUCT *pM4uPort);
int m4u_config_port_array(struct m4u_port_array *port_array);
int m4u_monitor_start(int m4u_id);
int m4u_monitor_stop(int m4u_id);

struct m4u_client_t *m4u_create_client(void);
int m4u_destroy_client(struct m4u_client_t *client);

int m4u_dump_reg_for_smi_hang_issue(void);
int m4u_dump_reg_for_vpu_hang_issue(void);
int m4u_display_fake_engine_test(unsigned long ulFakeReadAddr,
				 unsigned long ulFakeWriteAddr);

void m4u_larb_backup(int larb_idx);
void m4u_larb_restore(int larb_idx);


typedef enum m4u_callback_ret_t(m4u_reclaim_mva_callback_t) (int alloc_port,
							unsigned int mva,
							unsigned int size,
							void *data);
int m4u_register_reclaim_callback(int port, m4u_reclaim_mva_callback_t *fn,
				  void *data);
int m4u_unregister_reclaim_callback(int port);


#ifdef CONFIG_PM
extern void mt_irq_set_sens(unsigned int irq, unsigned int sens);
extern void mt_irq_set_polarity(unsigned int irq, unsigned int polarity);
#endif

#ifdef M4U_TEE_SERVICE_ENABLE
extern int gM4U_L2_enable;
#endif

extern void show_pte(struct mm_struct *mm, unsigned long addr);

#ifdef M4U_PROFILE
extern mmp_event M4U_MMP_Events[M4U_MMP_MAX];
#endif

/* m4u driver internal use ----------- */

#endif
