/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __RPS_PERF_H__
#define __RPS_PERF_H__

int set_rps_map(struct netdev_rx_queue *queue, unsigned long rps_value);

#endif /* __RPS_PERF_H__ */
