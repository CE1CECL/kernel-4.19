/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MDLA_QOS_H__
#define __MDLA_QOS_H__

int mdla_cmd_qos_start(int core);
int mdla_cmd_qos_end(int core);

int mdla_qos_counter_start(unsigned int core);
int mdla_qos_counter_end(unsigned int core);
int mdla_qos_counter_init(void);
void mdla_qos_counter_destroy(void);

#endif
