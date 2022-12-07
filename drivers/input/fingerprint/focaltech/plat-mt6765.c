/**
 * plat-mt6765.c
 *
**/

#include <linux/stddef.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/spi/spi.h>
#include <linux/platform_data/spi-mt65xx.h>
#include <linux/clk.h>
#include "ff_log.h"

#undef  LOG_TAG
#define LOG_TAG "mt6765"

/* TODO: */
#define FF_COMPATIBLE_NODE_1 "mediatek,fingerprint"
#define FF_COMPATIBLE_NODE_2 "mediatek,fingerprint"

/* Define pinctrl state types. */
typedef enum {
    FF_PINCTRL_STATE_RST_CLR,
    FF_PINCTRL_STATE_RST_ACT,
    FF_PINCTRL_STATE_INT,
    FF_PINCTRL_STATE_POWER_ON,
    FF_PINCTRL_STATE_POWER_OFF,
    FF_PINCTRL_STATE_MAXIMUM /* Array size */
} ff_pinctrl_state_t;
static const char *g_pinctrl_state_names[FF_PINCTRL_STATE_MAXIMUM] = {
    "fp_reset_low",
    "fp_reset_high",
    "fp_irq",
    "fp_power_on",
    "fp_power_off",
};

extern struct spi_master *spi_ctl;
static int ff_spi_probe(struct spi_device *dev);
static int ff_spi_remove(struct spi_device *spi);
extern void mt_spi_enable_master_clk(struct spi_device *spidev);
extern void mt_spi_disable_master_clk(struct spi_device *spidev);

/* Native context and its singleton instance. */
typedef struct {
    struct pinctrl *pinctrl;
    struct pinctrl_state *pin_states[FF_PINCTRL_STATE_MAXIMUM];
    bool b_spiclk_enabled;
	struct spi_device *spidev;
} ff_mt6765_context_t;
static ff_mt6765_context_t ff_mt6765_context, *g_context = &ff_mt6765_context;

static struct of_device_id  ff_spi_of_match[] = {
    { .compatible = "mediatek,fingerprint-spi-dev", },
};

static struct spi_driver ff_spi_driver = {
    .driver = {
        .name = "ff-spi",
        .bus = &spi_bus_type,
        .owner = THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = ff_spi_of_match,
#endif
    },
    .probe  = ff_spi_probe,
    .remove = ff_spi_remove,
};

static int ff_spi_probe(struct spi_device *dev)
{
	printk("%s enter\n", __func__);
	g_context->spidev = dev;
	return 0;
}

static int ff_spi_remove(struct spi_device *spi)
{
	return 0;
}
char fp_info_module_name[32]="NA";
static ssize_t fp_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("fp_module_name = %s\n",fp_info_module_name);
	if (!dev )
	{
		return 0;
	}
	return sprintf(buf, "%s\n", fp_info_module_name);
}
static DEVICE_ATTR(fp, 0444, fp_show, NULL);
extern struct device* get_deviceinfo_dev(void);
static void Create_FP_info_node(void)
{
	struct device * chipinfo;
	chipinfo = get_deviceinfo_dev();
	if(chipinfo != NULL){
	    if (device_create_file(chipinfo, &dev_attr_fp) < 0)
	    {
            printk("Failed to create device file(%s)!\n", dev_attr_fp.attr.name);
	    }
	}
    return;
}
static int fp_init_flag = 0;
int ff_ctl_init_pins(int *irq_num)
{
    int err = 0, i;
/*begin modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    static int save_irq_num = 0;
/*end modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    struct device_node *dev_node = NULL;
    struct platform_device *pdev = NULL;
    FF_LOGI("'%s' enter.", __func__);

    /* Find device tree node. */
    dev_node = of_find_compatible_node(NULL, NULL, FF_COMPATIBLE_NODE_1);
    if (!dev_node) {
        FF_LOGE("of_find_compatible_node(.., '%s') failed.", FF_COMPATIBLE_NODE_1);
        return (-ENODEV);
    }

    /* Convert to platform device. */
    pdev = of_find_device_by_node(dev_node);
    if (!pdev) {
        FF_LOGE("of_find_device_by_node(..) failed.");
        return (-ENODEV);
    }

    /* Retrieve the pinctrl handler. */
    g_context->pinctrl = devm_pinctrl_get(&pdev->dev);
    if (!g_context->pinctrl) {
        FF_LOGE("devm_pinctrl_get(..) failed.");
        return (-ENODEV);
    }

    /* Register all pins. */
    for (i = 0; i < FF_PINCTRL_STATE_MAXIMUM; ++i) {
        g_context->pin_states[i] = pinctrl_lookup_state(g_context->pinctrl, g_pinctrl_state_names[i]);
        if (!g_context->pin_states[i]) {
            FF_LOGE("can't find pinctrl state for '%s'.", g_pinctrl_state_names[i]);
            err = (-ENODEV);
            break;
        }
    }
    if (i < FF_PINCTRL_STATE_MAXIMUM) {
        return (-ENODEV);
    }

    /* Initialize the INT pin. */
    err = pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_INT]);

    /* Retrieve the irq number. */
    dev_node = of_find_compatible_node(NULL, NULL, FF_COMPATIBLE_NODE_2);
    if (!dev_node) {
        FF_LOGE("of_find_compatible_node(.., '%s') failed.", FF_COMPATIBLE_NODE_2);
        return (-ENODEV);
    }
/*begin modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    if(fp_init_flag==0){
        *irq_num = irq_of_parse_and_map(dev_node, 0);
        save_irq_num = *irq_num;
    }else{
        if(save_irq_num > 0)
            *irq_num = save_irq_num;
        else
            *irq_num = irq_of_parse_and_map(dev_node, 0);
    }
/*end modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    FF_LOGI("irq number is %d.", *irq_num);
    /* Initialize the RESET pin. */
    pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_RST_ACT]);

	err = spi_register_driver(&ff_spi_driver);
	if (err < 0)
	{
		printk("%s register spi driver fail\n", __func__);
	}

    FF_LOGI("'%s' leave.", __func__);
/*begin modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    if(fp_init_flag==0){
        Create_FP_info_node();
        fp_init_flag ++;
    }
/*end modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    strcpy(fp_info_module_name,"FT");
    return err;
}

int ff_ctl_free_pins(void)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);
	printk("%s: devm pinctrl put\n", __func__);
/*begin modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    if(g_context->pinctrl != NULL){
         devm_pinctrl_put(g_context->pinctrl);
         spi_unregister_driver(&ff_spi_driver);
         g_context->pinctrl = NULL;
    }
/*end modified by zhikui.li,Solve the problem of focal re-initialization failure*/
    // TODO:
    strcpy(fp_info_module_name,"NA");
    FF_LOGV("'%s' leave.", __func__);
    return err;
}

int ff_ctl_enable_spiclk(bool on)
{
    int err = 0;

    //FF_LOGI("'%s' enter.", __func__);
    //FF_LOGD("clock: '%s'.", on ? "enable" : "disabled");

    FF_LOGI("b_spiclk_enabled = %d. on=%d", g_context->b_spiclk_enabled,on);
    /* Control the clock source. */
    if (on && !g_context->b_spiclk_enabled) {
		mt_spi_enable_master_clk(g_context->spidev);
        g_context->b_spiclk_enabled = true;
    } else if (!on && g_context->b_spiclk_enabled) {
		mt_spi_disable_master_clk(g_context->spidev);
        g_context->b_spiclk_enabled = false;
    }

    //FF_LOGI("'%s' leave.", __func__);
    return err;
}

int ff_ctl_enable_power(bool on)
{
    int err = 0;
    FF_LOGI("'%s' enter.", __func__);
    FF_LOGD("power: '%s'.", on ? "on" : "off");

    if (unlikely(!g_context->pinctrl)) {
        return (-ENOSYS);
    }

    if (on) {
        err = pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_POWER_ON]);
    } else {
        err = pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_POWER_OFF]);
    }

    FF_LOGI("'%s' leave.", __func__);
    return err;
}

int ff_ctl_reset_device(void)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);

    if (unlikely(!g_context->pinctrl)) {
        return (-ENOSYS);
    }

    /* 3-1: Pull down RST pin. */
    err = pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_RST_CLR]);

    /* 3-2: Delay for 10ms. */
    mdelay(10);

    /* Pull up RST pin. */
    err = pinctrl_select_state(g_context->pinctrl, g_context->pin_states[FF_PINCTRL_STATE_RST_ACT]);

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

const char *ff_ctl_arch_str(void)
{
    return CONFIG_MTK_PLATFORM;
}

