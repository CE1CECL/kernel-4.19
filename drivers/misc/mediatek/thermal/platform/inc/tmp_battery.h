/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __TMP_BATTERY_H__
#define __TMP_BATTERY_H__

#include <charging.h>
extern int get_bat_charging_current_level(void);

extern int set_bat_charging_current_limit(int current_limit);
extern enum charger_type mt_get_charger_type(void);

extern int read_tbat_value(void);

#endif	/* __TMP_BATTERY_H__ */

