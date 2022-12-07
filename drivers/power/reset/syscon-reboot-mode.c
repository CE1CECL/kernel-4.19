/*
 * Copyright (c) 2016, Fuzhou Rockchip Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/reboot-mode.h>

/* Begin added by mingbo.feng for defect-10428894  on 2020-12-02 */
#if defined(JRD_PROJECT_FULL_BANGKOK_TF) || defined(JRD_PROJECT_VND_BANGKOK_TF) \
	|| defined(JRD_PROJECT_FULL_BANGKOK_NA_OM) || defined(JRD_PROJECT_VND_BANGKOK_NA_OM)
#if defined(CONFIG_MTK_PMIC_WRAP)
#include <linux/regmap.h>
#include <linux/soc/mediatek/pmic_wrap.h>
#include <linux/mfd/mt6357/registers.h>
static struct regmap *regmap;
#endif
#define RTC_PDN1_RECOVERY_MASK      0x0030
#define RTC_PDN1_FAC_RESET      (1U << 4)
#define RTC_PDN1_FAST_BOOT      (1U << 13)
#endif
/* End added by mingbo.feng for defect-10428894  on 2020-12-02 */
struct syscon_reboot_mode {
	struct regmap *map;
	struct reboot_mode_driver reboot;
	u32 offset;
	u32 mask;
};

static int syscon_reboot_mode_write(struct reboot_mode_driver *reboot,
				    unsigned int magic)
{
	struct syscon_reboot_mode *syscon_rbm;
	int ret;
/* Begin added by mingbo.feng for defect-10428894  on 2020-12-02 */
#if defined(JRD_PROJECT_FULL_BANGKOK_TF) || defined(JRD_PROJECT_VND_BANGKOK_TF) \
	|| defined(JRD_PROJECT_FULL_BANGKOK_NA_OM) || defined(JRD_PROJECT_VND_BANGKOK_NA_OM)
	int reg_val=0;
#endif
/* End added by mingbo.feng for defect-10428894  on 2020-12-02 */

	syscon_rbm = container_of(reboot, struct syscon_reboot_mode, reboot);

	ret = regmap_update_bits(syscon_rbm->map, syscon_rbm->offset,
				 syscon_rbm->mask, magic);
	if (ret < 0)
		dev_err(reboot->dev, "update reboot mode bits failed\n");

/* Begin added by mingbo.feng for defect-10428894  on 2020-12-02 */
#if defined(JRD_PROJECT_FULL_BANGKOK_TF) || defined(JRD_PROJECT_VND_BANGKOK_TF) \
	|| defined(JRD_PROJECT_FULL_BANGKOK_NA_OM) || defined(JRD_PROJECT_VND_BANGKOK_NA_OM)
        if(magic ==3){//for bootloader
            regmap_read(regmap, MT6357_RTC_PDN1_ADDR, &reg_val);
            pr_err(" Bootloader: MT6357_RTC_PDN1  =%x\n",reg_val);

            reg_val = reg_val | RTC_PDN1_FAST_BOOT;
           regmap_update_bits(regmap, MT6357_RTC_PDN1_ADDR,MT6357_RTC_PDN1_MASK,reg_val);
	    msleep(1);
	    regmap_update_bits(regmap, MT6357_RTC_WRTGR,MT6357_WRTGR_MASK, 1);

            msleep(1);
	    regmap_update_bits(regmap, MT6357_RG_CRST_ADDR,MT6357_RG_CRST_MASK<<MT6357_RG_CRST_SHIFT, 1<<MT6357_RG_CRST_SHIFT);
	}

      if(magic ==2){//for recovery

            regmap_read(regmap, MT6357_RTC_PDN1_ADDR, &reg_val);
            pr_err("Recovery: MT6357_RTC_PDN1 =%x\n",reg_val);

            reg_val = reg_val | RTC_PDN1_FAC_RESET;
            regmap_update_bits(regmap, MT6357_RTC_PDN1_ADDR,MT6357_RTC_PDN1_MASK,reg_val);
	    msleep(1);
            regmap_update_bits(regmap, MT6357_RTC_WRTGR,MT6357_WRTGR_MASK, 1);

	    msleep(1);
            regmap_update_bits(regmap, MT6357_RG_CRST_ADDR,MT6357_RG_CRST_MASK<<MT6357_RG_CRST_SHIFT, 1<<MT6357_RG_CRST_SHIFT);
	}
#endif
/* End added by mingbo.feng for 10428894  on 2020-12-02 */

	return ret;
}

static int syscon_reboot_mode_probe(struct platform_device *pdev)
{
	int ret;
	struct syscon_reboot_mode *syscon_rbm;
/* Begin added by mingbo.feng for defect-  on 2020-12-02 */
#if defined(JRD_PROJECT_FULL_BANGKOK_TF) || defined(JRD_PROJECT_VND_BANGKOK_TF) \
	|| defined(JRD_PROJECT_FULL_BANGKOK_NA_OM) || defined(JRD_PROJECT_VND_BANGKOK_NA_OM)
#if defined(CONFIG_MTK_PMIC_WRAP)
        struct device_node *pwrap_node;
#endif
#endif
/* End added by mingbo.feng for defect-10428894  on 2020-12-02 */


	syscon_rbm = devm_kzalloc(&pdev->dev, sizeof(*syscon_rbm), GFP_KERNEL);
	if (!syscon_rbm)
		return -ENOMEM;

	syscon_rbm->reboot.dev = &pdev->dev;
	syscon_rbm->reboot.write = syscon_reboot_mode_write;
	syscon_rbm->mask = 0xffffffff;

	syscon_rbm->map = syscon_node_to_regmap(pdev->dev.parent->of_node);
	if (IS_ERR(syscon_rbm->map))
		return PTR_ERR(syscon_rbm->map);

	if (of_property_read_u32(pdev->dev.of_node, "offset",
	    &syscon_rbm->offset))
		return -EINVAL;

	of_property_read_u32(pdev->dev.of_node, "mask", &syscon_rbm->mask);

/* Begin added by mingbo.feng for defect- 10428894 on 2020-12-02 */
#if defined(JRD_PROJECT_FULL_BANGKOK_TF) || defined(JRD_PROJECT_VND_BANGKOK_TF) \
	|| defined(JRD_PROJECT_FULL_BANGKOK_NA_OM) || defined(JRD_PROJECT_VND_BANGKOK_NA_OM)
#if defined(CONFIG_MTK_PMIC_WRAP)
	pwrap_node = of_parse_phandle(pdev->dev.of_node,"mediatek,pwrap-regmap", 0);
	if (!pwrap_node)
		return -ENODEV;
	regmap = pwrap_node_to_regmap(pwrap_node);
#endif
#endif
/* End added by mingbo.feng for defect-10428894  on 2020-12-02 */

	ret = devm_reboot_mode_register(&pdev->dev, &syscon_rbm->reboot);
	if (ret)
		dev_err(&pdev->dev, "can't register reboot mode\n");

	return ret;
}

static const struct of_device_id syscon_reboot_mode_of_match[] = {
	{ .compatible = "syscon-reboot-mode" },
	{}
};
MODULE_DEVICE_TABLE(of, syscon_reboot_mode_of_match);

static struct platform_driver syscon_reboot_mode_driver = {
	.probe = syscon_reboot_mode_probe,
	.driver = {
		.name = "syscon-reboot-mode",
		.of_match_table = syscon_reboot_mode_of_match,
	},
};
module_platform_driver(syscon_reboot_mode_driver);

MODULE_AUTHOR("Andy Yan <andy.yan@rock-chips.com");
MODULE_DESCRIPTION("SYSCON reboot mode driver");
MODULE_LICENSE("GPL v2");
