/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __SPMTWAM_H__
#define __SPMTWAM_H__

#define MONTYPE_RISING              (0)
#define MONTYPE_FALLING             (1)
#define MONTYPE_HIGH_LEVEL          (2)
#define MONITPE_LOW_LEVEL           (3)

#define SPEED_MODE_NORMAL           (0)
#define SPEED_MODE_SPEED            (1)

#define DEFAULT_MONTYPE             MONTYPE_HIGH_LEVEL
#define DEFAULT_SPEED_MODE          SPEED_MODE_SPEED

#define TWAM_PERIOD_MS              (1000)
#define WINDOW_LEN_SPEED            (TWAM_PERIOD_MS * 0x65B8)
#define WINDOW_LEN_NORMAL           (TWAM_PERIOD_MS * 0xD)
#define GET_EVENT_RATIO_SPEED(x)    ((x)/(WINDOW_LEN_SPEED/1000))
#define GET_EVENT_RATIO_NORMAL(x)   ((x)/(WINDOW_LEN_NORMAL/1000))


struct spmtwam_channel {
	u32 signal;    /* 2 bits, signal A~D */
	u32 id;        /* 5 bits, id 0~31    */
	u32 montype;   /* 2 bits, monitor type */
};

struct spmtwam_cfg {
	struct spmtwam_channel ch[4];   /* channel 0~3 config */
	u32 spmtwam_speed_mode;         /* 0: normal, 1: high speed */
	u32 spmtwam_window_len;
	/* spmtwam_window_len
	 *	0: will be automatically updated for current speed mode
	 *  for normal speed mode, set to WINDOW_LEN_NORMAL.
	 *  for high speed mode, set to WINDOW_LEN_SPEED.
	 */
};

/* definitions for return result */
struct spmtwam_result {
	struct spmtwam_cfg cfg;
	u32 value[4];
};

typedef void (*spmtwam_handler_t)(struct spmtwam_result *result);

int spmtwam_monitor(bool enable, struct spmtwam_cfg *cfg,
	spmtwam_handler_t handler);


#endif /* __SPMTWAM_H__ */
