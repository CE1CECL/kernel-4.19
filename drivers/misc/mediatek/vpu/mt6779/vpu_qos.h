/* SPDX-License-Identifier: GPL-2.0 */




#ifndef _MT6779_VPU_QOS_H_
#define _MT6779_VPU_QOS_H_

int vpu_cmd_qos_start(int core);
int vpu_cmd_qos_end(int core);

int vpu_qos_counter_start(unsigned int core);
int vpu_qos_counter_end(unsigned int core);
int vpu_qos_counter_init(void);
void vpu_qos_counter_destroy(void);

#endif
