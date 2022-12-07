/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _MT753X_H_
#define _MT753X_H_

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/of_mdio.h>
#include <linux/workqueue.h>
#include <linux/gpio/consumer.h>

#ifdef CONFIG_SWCONFIG
#include <linux/switch.h>
#endif

#include "mt753x_vlan.h"

#define MT753X_DFL_CPU_PORT	6
#define MT753X_NUM_PHYS		5

#define MT753X_DFL_SMI_ADDR	0x1f
#define MT753X_SMI_ADDR_MASK	0x1f

struct gsw_mt753x;

enum mt753x_model {
	MT7530 = 0x7530,
	MT7531 = 0x7531
};

struct mt753x_port_cfg {
	struct device_node *np;
	int phy_mode;
	u32 enabled: 1;
	u32 force_link: 1;
	u32 speed: 2;
	u32 duplex: 1;
};

struct mt753x_phy {
	struct gsw_mt753x *gsw;
	struct net_device netdev;
	struct phy_device *phydev;
};

struct gsw_mt753x {
	u32 id;

	struct device *dev;
	struct mii_bus *host_bus;
	struct mii_bus *gphy_bus;
	struct mutex mii_lock;	/* MII access lock */
	u32 smi_addr;
	u32 phy_base;
	int direct_phy_access;

	enum mt753x_model model;
	const char *name;

	struct mt753x_port_cfg port5_cfg;
	struct mt753x_port_cfg port6_cfg;

	int phy_status_poll;
	struct mt753x_phy phys[MT753X_NUM_PHYS];

	int phy_link_sts;

	int irq;
	int reset_pin;
	struct work_struct irq_worker;

#ifdef CONFIG_SWCONFIG
	struct switch_dev swdev;
	u32 cpu_port;
#endif

	int global_vlan_enable;
	struct mt753x_vlan_entry vlan_entries[MT753X_NUM_VLANS];
	struct mt753x_port_entry port_entries[MT753X_NUM_PORTS];

	int (*mii_read)(struct gsw_mt753x *gsw, int phy, int reg);
	void (*mii_write)(struct gsw_mt753x *gsw, int phy, int reg, u16 val);

	int (*mmd_read)(struct gsw_mt753x *gsw, int addr, int devad, u16 reg);
	void (*mmd_write)(struct gsw_mt753x *gsw, int addr, int devad, u16 reg,
			  u16 val);

	struct list_head list;
};

struct chip_rev {
	const char *name;
	u32 rev;
};

struct mt753x_sw_id {
	enum mt753x_model model;
	int (*detect)(struct gsw_mt753x *gsw, struct chip_rev *crev);
	int (*init)(struct gsw_mt753x *gsw);
	int (*post_init)(struct gsw_mt753x *gsw);
};

extern struct list_head mt753x_devs;

struct gsw_mt753x *mt753x_get_gsw(u32 id);
struct gsw_mt753x *mt753x_get_first_gsw(void);
void mt753x_put_gsw(void);
void mt753x_lock_gsw(void);

u32 mt753x_reg_read(struct gsw_mt753x *gsw, u32 reg);
void mt753x_reg_write(struct gsw_mt753x *gsw, u32 reg, u32 val);

int mt753x_mii_read(struct gsw_mt753x *gsw, int phy, int reg);
void mt753x_mii_write(struct gsw_mt753x *gsw, int phy, int reg, u16 val);

int mt753x_mmd_read(struct gsw_mt753x *gsw, int addr, int devad, u16 reg);
void mt753x_mmd_write(struct gsw_mt753x *gsw, int addr, int devad, u16 reg,
		      u16 val);

int mt753x_mmd_ind_read(struct gsw_mt753x *gsw, int addr, int devad, u16 reg);
void mt753x_mmd_ind_write(struct gsw_mt753x *gsw, int addr, int devad, u16 reg,
			  u16 val);

void mt753x_irq_worker(struct work_struct *work);
void mt753x_irq_enable(struct gsw_mt753x *gsw);

/* MDIO Indirect Access Registers */
#define MII_MMD_ACC_CTL_REG		0x0d
#define MMD_CMD_S			14
#define MMD_CMD_M			0xc000
#define MMD_DEVAD_S			0
#define MMD_DEVAD_M			0x1f

/* MMD_CMD: MMD commands */
#define MMD_ADDR			0
#define MMD_DATA			1

#define MII_MMD_ADDR_DATA_REG		0x0e


/* Internal Register Address fields */
#define MT753X_REG_PAGE_ADDR_S		6
#define MT753X_REG_PAGE_ADDR_M		0xffc0
#define MT753X_REG_ADDR_S		2
#define MT753X_REG_ADDR_M		0x3c
#endif /* _MT753X_H_ */
