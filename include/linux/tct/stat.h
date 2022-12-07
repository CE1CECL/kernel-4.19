#ifndef _TCT_STAT_H
#define _TCT_STAT_H

enum {
	UI_IOWAIT,
	NR_TCT_STAT,
};

void tctstat_account_delay(u64 value, int type);
#endif
