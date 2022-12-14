
#ifndef __PINCTRL_MTK_COMMON_H
#define __PINCTRL_MTK_COMMON_H

#include <linux/pinctrl/pinctrl.h>
#include <linux/regmap.h>
#include <linux/pinctrl/pinconf-generic.h>

#include "mtk-eint.h"

#define NO_EINT_SUPPORT    255
#define MT_EDGE_SENSITIVE           0
#define MT_LEVEL_SENSITIVE          1
#define EINT_DBNC_SET_DBNC_BITS     4
#define EINT_DBNC_RST_BIT           (0x1 << 1)
#define EINT_DBNC_SET_EN            (0x1 << 0)

#define MTK_PINCTRL_NOT_SUPPORT	(0xffff)

struct mtk_desc_function {
	const char *name;
	unsigned char muxval;
};

struct mtk_desc_eint {
	unsigned char eintmux;
	unsigned char eintnum;
};

struct mtk_desc_pin {
	struct pinctrl_pin_desc	pin;
	const struct mtk_desc_eint eint;
	const struct mtk_desc_function	*functions;
};

#define MTK_PIN(_pin, _pad, _chip, _eint, ...)		\
	{							\
		.pin = _pin,					\
		.eint = _eint,					\
		.functions = (struct mtk_desc_function[]){	\
			__VA_ARGS__, { } },			\
	}

#define MTK_EINT_FUNCTION(_eintmux, _eintnum)				\
	{							\
		.eintmux = _eintmux,					\
		.eintnum = _eintnum,					\
	}

#define MTK_FUNCTION(_val, _name)				\
	{							\
		.muxval = _val,					\
		.name = _name,					\
	}

#define SET_ADDR(x, y)  (x + (y->devdata->port_align))
#define CLR_ADDR(x, y)  (x + (y->devdata->port_align << 1))

struct mtk_pinctrl_group {
	const char	*name;
	unsigned long	config;
	unsigned	pin;
};

struct mtk_drv_group_desc {
	unsigned char min_drv;
	unsigned char max_drv;
	unsigned char low_bit;
	unsigned char high_bit;
	unsigned char step;
};

#define MTK_DRV_GRP(_min, _max, _low, _high, _step)	\
	{	\
		.min_drv = _min,	\
		.max_drv = _max,	\
		.low_bit = _low,	\
		.high_bit = _high,	\
		.step = _step,		\
	}

struct mtk_pin_drv_grp {
	unsigned short pin;
	unsigned short offset;
	unsigned char bit;
	unsigned char grp;
};

#define MTK_PIN_DRV_GRP(_pin, _offset, _bit, _grp)	\
	{	\
		.pin = _pin,	\
		.offset = _offset,	\
		.bit = _bit,	\
		.grp = _grp,	\
	}

struct mtk_pin_spec_pupd_set_samereg {
	unsigned short pin;
	unsigned short offset;
	unsigned char pupd_bit;
	unsigned char r1_bit;
	unsigned char r0_bit;
};

#define MTK_PIN_PUPD_SPEC_SR(_pin, _offset, _pupd, _r1, _r0)	\
	{	\
		.pin = _pin,	\
		.offset = _offset,	\
		.pupd_bit = _pupd,	\
		.r1_bit = _r1,		\
		.r0_bit = _r0,		\
	}

struct mtk_pin_ies_smt_set {
	unsigned short start;
	unsigned short end;
	unsigned short offset;
	unsigned char bit;
};

#define MTK_PIN_IES_SMT_SPEC(_start, _end, _offset, _bit)	\
	{	\
		.start = _start,	\
		.end = _end,	\
		.bit = _bit,	\
		.offset = _offset,	\
	}

struct mtk_eint_offsets {
	const char *name;
	unsigned int  stat;
	unsigned int  ack;
	unsigned int  mask;
	unsigned int  mask_set;
	unsigned int  mask_clr;
	unsigned int  sens;
	unsigned int  sens_set;
	unsigned int  sens_clr;
	unsigned int  soft;
	unsigned int  soft_set;
	unsigned int  soft_clr;
	unsigned int  pol;
	unsigned int  pol_set;
	unsigned int  pol_clr;
	unsigned int  dom_en;
	unsigned int  dbnc_ctrl;
	unsigned int  dbnc_set;
	unsigned int  dbnc_clr;
	u8  port_mask;
	u8  ports;
};

struct mtk_pinctrl_devdata {
	const struct mtk_desc_pin	*pins;
	unsigned int				npins;
	const struct mtk_drv_group_desc	*grp_desc;
	unsigned int	n_grp_cls;
	const struct mtk_pin_drv_grp	*pin_drv_grp;
	unsigned int	n_pin_drv_grps;
	int (*spec_pull_set)(struct regmap *reg, unsigned int pin,
			unsigned char align, bool isup, unsigned int arg);
	int (*spec_ies_smt_set)(struct regmap *reg, unsigned int pin,
			unsigned char align, int value, enum pin_config_param arg);
	void (*spec_pinmux_set)(struct regmap *reg, unsigned int pin,
			unsigned int mode);
	void (*spec_dir_set)(unsigned int *reg_addr, unsigned int pin);
	unsigned int dir_offset;
	unsigned int ies_offset;
	unsigned int smt_offset;
	unsigned int pullen_offset;
	unsigned int pullsel_offset;
	unsigned int drv_offset;
	unsigned int dout_offset;
	unsigned int din_offset;
	unsigned int pinmux_offset;
	unsigned short type1_start;
	unsigned short type1_end;
	unsigned char  port_shf;
	unsigned char  port_mask;
	unsigned char  port_align;
	struct mtk_eint_hw eint_hw;
	struct mtk_eint_regs *eint_regs;
};

struct mtk_pinctrl {
	struct regmap	*regmap1;
	struct regmap	*regmap2;
	struct pinctrl_desc pctl_desc;
	struct device           *dev;
	struct gpio_chip	*chip;
	struct mtk_pinctrl_group	*groups;
	unsigned			ngroups;
	const char          **grp_names;
	struct pinctrl_dev      *pctl_dev;
	const struct mtk_pinctrl_devdata  *devdata;
	struct mtk_eint *eint;
};

int mtk_pctrl_init(struct platform_device *pdev,
		const struct mtk_pinctrl_devdata *data,
		struct regmap *regmap);

int mtk_pctrl_spec_pull_set_samereg(struct regmap *regmap,
		const struct mtk_pin_spec_pupd_set_samereg *pupd_infos,
		unsigned int info_num, unsigned int pin,
		unsigned char align, bool isup, unsigned int r1r0);

int mtk_pconf_spec_set_ies_smt_range(struct regmap *regmap,
		const struct mtk_pin_ies_smt_set *ies_smt_infos, unsigned int info_num,
		unsigned int pin, unsigned char align, int value);

extern const struct dev_pm_ops mtk_eint_pm_ops;

#endif /* __PINCTRL_MTK_COMMON_H */
