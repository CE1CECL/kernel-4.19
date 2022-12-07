// SPDX-License-Identifier: GPL-2.0

#ifndef __MTU3_H__
#define __MTU3_H__

#include <linux/device.h>
#include <linux/dmapool.h>
#include <linux/extcon.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/phy/phy.h>
#include <linux/regulator/consumer.h>
#include <linux/usb.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/otg.h>
#include <linux/usb/role.h>

struct mtu3;
struct mtu3_ep;
struct mtu3_request;

#include "mtu3_hw_regs.h"
#include "mtu3_qmu.h"

#define	MU3D_EP_TXCR0(epnum)	(U3D_TX1CSR0 + (((epnum) - 1) * 0x10))
#define	MU3D_EP_TXCR1(epnum)	(U3D_TX1CSR1 + (((epnum) - 1) * 0x10))
#define	MU3D_EP_TXCR2(epnum)	(U3D_TX1CSR2 + (((epnum) - 1) * 0x10))

#define	MU3D_EP_RXCR0(epnum)	(U3D_RX1CSR0 + (((epnum) - 1) * 0x10))
#define	MU3D_EP_RXCR1(epnum)	(U3D_RX1CSR1 + (((epnum) - 1) * 0x10))
#define	MU3D_EP_RXCR2(epnum)	(U3D_RX1CSR2 + (((epnum) - 1) * 0x10))

#define USB_QMU_TQHIAR(epnum)	(U3D_TXQHIAR1 + (((epnum) - 1) * 0x4))
#define USB_QMU_RQHIAR(epnum)	(U3D_RXQHIAR1 + (((epnum) - 1) * 0x4))

#define USB_QMU_RQCSR(epnum)	(U3D_RXQCSR1 + (((epnum) - 1) * 0x10))
#define USB_QMU_RQSAR(epnum)	(U3D_RXQSAR1 + (((epnum) - 1) * 0x10))
#define USB_QMU_RQCPR(epnum)	(U3D_RXQCPR1 + (((epnum) - 1) * 0x10))

#define USB_QMU_TQCSR(epnum)	(U3D_TXQCSR1 + (((epnum) - 1) * 0x10))
#define USB_QMU_TQSAR(epnum)	(U3D_TXQSAR1 + (((epnum) - 1) * 0x10))
#define USB_QMU_TQCPR(epnum)	(U3D_TXQCPR1 + (((epnum) - 1) * 0x10))

#define SSUSB_U3_CTRL(p)	(U3D_SSUSB_U3_CTRL_0P + ((p) * 0x08))
#define SSUSB_U2_CTRL(p)	(U3D_SSUSB_U2_CTRL_0P + ((p) * 0x08))

#define MTU3_DRIVER_NAME	"mtu3"
#define	DMA_ADDR_INVALID	(~(dma_addr_t)0)

#define MTU3_EP_ENABLED		BIT(0)
#define MTU3_EP_STALL		BIT(1)
#define MTU3_EP_WEDGE		BIT(2)
#define MTU3_EP_BUSY		BIT(3)

#define MTU3_U3_IP_SLOT_DEFAULT 2
#define MTU3_U2_IP_SLOT_DEFAULT 1

#define MTU3_SW_ID_GROUND	BIT(0)
#define MTU3_SW_VBUS_VALID	BIT(1)

#define MTU3_TRUNK_VERS_1003	0x1003

#define MTU3_EP_FIFO_UNIT		(1 << 9)
#define MTU3_FIFO_BIT_SIZE		128
#define MTU3_U2_IP_EP0_FIFO_SIZE	64

#define EP0_RESPONSE_BUF  6

/* device operated link and speed got from DEVICE_CONF register */
enum mtu3_speed {
	MTU3_SPEED_INACTIVE = 0,
	MTU3_SPEED_FULL = 1,
	MTU3_SPEED_HIGH = 3,
	MTU3_SPEED_SUPER = 4,
	MTU3_SPEED_SUPER_PLUS = 5,
};

enum mtu3_g_ep0_state {
	MU3D_EP0_STATE_SETUP = 1,
	MU3D_EP0_STATE_TX,
	MU3D_EP0_STATE_RX,
	MU3D_EP0_STATE_TX_END,
	MU3D_EP0_STATE_STALL,
};

enum mtu3_dr_force_mode {
	MTU3_DR_FORCE_NONE = 0,
	MTU3_DR_FORCE_HOST,
	MTU3_DR_FORCE_DEVICE,
};

enum mtu3_dr_operation_mode {
	MTU3_DR_OPERATION_NONE = 0,
	MTU3_DR_OPERATION_NORMAL,
	MTU3_DR_OPERATION_HOST,
	MTU3_DR_OPERATION_DEVICE,
};

struct mtu3_fifo_info {
	u32 base;
	u32 limit;
	DECLARE_BITMAP(bitmap, MTU3_FIFO_BIT_SIZE);
};

struct qmu_gpd {
	__le32 dw0_info;
	__le32 next_gpd;
	__le32 buffer;
	__le32 dw3_info;
} __packed;

struct mtu3_gpd_ring {
	dma_addr_t dma;
	struct qmu_gpd *start;
	struct qmu_gpd *end;
	struct qmu_gpd *enqueue;
	struct qmu_gpd *dequeue;
};

struct otg_switch_mtk {
	struct regulator *vbus;
	struct extcon_dev *edev;
	struct notifier_block vbus_nb;
	struct work_struct vbus_work;
	unsigned long vbus_event;
	struct notifier_block id_nb;
	struct work_struct id_work;
	unsigned long id_event;
	struct usb_role_switch *role_sw;
	bool role_sw_used;
	bool is_u3_drd;
	bool manual_drd_enabled;
	u32 sw_state;
	enum usb_role latest_role;
	enum mtu3_dr_operation_mode op_mode;
};

struct ssusb_mtk {
	struct device *dev;
	struct mtu3 *u3d;
	void __iomem *mac_base;
	void __iomem *ippc_base;
	struct phy **phys;
	int num_phys;
	/* common power & clock */
	struct regulator *vusb33;
	struct clk *sys_clk;
	struct clk *ref_clk;
	struct clk *mcu_clk;
	struct clk *dma_clk;
	/* otg */
	struct otg_switch_mtk otg_switch;
	enum usb_dr_mode dr_mode;
	bool is_host;
	int u2_ports;
	int u3_ports;
	int u3p_dis_msk;
	struct dentry *dbgfs_root;
	bool force_vbus;
	/* usb wakeup for host mode */
	bool uwk_en;
	struct regmap *uwk;
	u32 uwk_reg_base;
	u32 uwk_vers;
	bool clk_on;
	bool clk_mgr;
	bool spm_mgr;
};

struct mtu3_ep {
	struct usb_ep ep;
	char name[12];
	struct mtu3 *mtu;
	u8 epnum;
	u8 type;
	u8 is_in;
	u16 maxp;
	int slot;
	u32 fifo_size;
	u32 fifo_addr;
	u32 fifo_seg_size;
	struct mtu3_fifo_info *fifo;

	struct list_head req_list;
	struct mtu3_gpd_ring gpd_ring;
	const struct usb_ss_ep_comp_descriptor *comp_desc;
	const struct usb_endpoint_descriptor *desc;

	int flags;
	u8 wedged;
	u8 busy;
};

struct mtu3_request {
	struct usb_request request;
	struct list_head list;
	struct mtu3_ep *mep;
	struct mtu3 *mtu;
	struct qmu_gpd *gpd;
	int epnum;
};

static inline struct ssusb_mtk *dev_to_ssusb(struct device *dev)
{
	return dev_get_drvdata(dev);
}

struct mtu3 {
	spinlock_t lock;
	struct ssusb_mtk *ssusb;
	struct device *dev;
	void __iomem *mac_base;
	void __iomem *ippc_base;
	int irq;

	struct mtu3_fifo_info tx_fifo;
	struct mtu3_fifo_info rx_fifo;

	struct mtu3_ep *ep_array;
	struct mtu3_ep *in_eps;
	struct mtu3_ep *out_eps;
	struct mtu3_ep *ep0;
	int num_eps;
	int slot;
	int active_ep;

	struct dma_pool	*qmu_gpd_pool;
	enum mtu3_g_ep0_state ep0_state;
	struct usb_gadget g;	/* the gadget */
	struct usb_gadget_driver *gadget_driver;
	struct mtu3_request ep0_req;
	u8 setup_buf[EP0_RESPONSE_BUF];
	u32 max_speed;

	unsigned is_active:1;
	unsigned may_wakeup:1;
	unsigned is_self_powered:1;
	unsigned test_mode:1;
	unsigned softconnect:1;
	unsigned u1_enable:1;
	unsigned u2_enable:1;
	unsigned is_u3_ip:1;
	unsigned delayed_status:1;
	unsigned gen2cp:1;

	u8 address;
	u8 test_mode_nr;
	u32 hw_version;

	unsigned is_gadget_ready:1;
};

static inline struct mtu3 *gadget_to_mtu3(struct usb_gadget *g)
{
	return container_of(g, struct mtu3, g);
}

static inline int is_first_entry(const struct list_head *list,
	const struct list_head *head)
{
	return list_is_last(head, list);
}

static inline struct mtu3_request *to_mtu3_request(struct usb_request *req)
{
	return req ? container_of(req, struct mtu3_request, request) : NULL;
}

static inline struct mtu3_ep *to_mtu3_ep(struct usb_ep *ep)
{
	return ep ? container_of(ep, struct mtu3_ep, ep) : NULL;
}

static inline struct mtu3_request *next_request(struct mtu3_ep *mep)
{
	return list_first_entry_or_null(&mep->req_list, struct mtu3_request,
					list);
}

static inline void mtu3_writel(void __iomem *base, u32 offset, u32 data)
{
	writel(data, base + offset);
}

static inline u32 mtu3_readl(void __iomem *base, u32 offset)
{
	return readl(base + offset);
}

static inline void mtu3_setbits(void __iomem *base, u32 offset, u32 bits)
{
	void __iomem *addr = base + offset;
	u32 tmp = readl(addr);

	writel((tmp | (bits)), addr);
}

static inline void mtu3_clrbits(void __iomem *base, u32 offset, u32 bits)
{
	void __iomem *addr = base + offset;
	u32 tmp = readl(addr);

	writel((tmp & ~(bits)), addr);
}

int ssusb_check_clocks(struct ssusb_mtk *ssusb, u32 ex_clks);
void ssusb_set_force_vbus(struct ssusb_mtk *ssusb, bool vbus_on);
int ssusb_phy_power_on(struct ssusb_mtk *ssusb);
void ssusb_phy_power_off(struct ssusb_mtk *ssusb);
int ssusb_clks_enable(struct ssusb_mtk *ssusb);
void ssusb_clks_disable(struct ssusb_mtk *ssusb);
void ssusb_ip_sw_reset(struct ssusb_mtk *ssusb);
struct usb_request *mtu3_alloc_request(struct usb_ep *ep, gfp_t gfp_flags);
void mtu3_free_request(struct usb_ep *ep, struct usb_request *req);
void mtu3_req_complete(struct mtu3_ep *mep,
		struct usb_request *req, int status);

int mtu3_config_ep(struct mtu3 *mtu, struct mtu3_ep *mep,
		int interval, int burst, int mult);
void mtu3_deconfig_ep(struct mtu3 *mtu, struct mtu3_ep *mep);
void mtu3_ep_stall_set(struct mtu3_ep *mep, bool set);
void mtu3_ep0_setup(struct mtu3 *mtu);
void mtu3_start(struct mtu3 *mtu);
void mtu3_stop(struct mtu3 *mtu);
void mtu3_dev_on_off(struct mtu3 *mtu, int is_on);
void mtu3_nuke_all_ep(struct mtu3 *mtu);

int mtu3_gadget_setup(struct mtu3 *mtu);
void mtu3_gadget_cleanup(struct mtu3 *mtu);
void mtu3_gadget_reset(struct mtu3 *mtu);
void mtu3_gadget_suspend(struct mtu3 *mtu);
void mtu3_gadget_resume(struct mtu3 *mtu);
void mtu3_gadget_disconnect(struct mtu3 *mtu);

irqreturn_t mtu3_ep0_isr(struct mtu3 *mtu);
extern const struct usb_ep_ops mtu3_ep0_ops;

#endif
