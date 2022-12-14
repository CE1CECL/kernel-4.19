#ifndef _MTK_CEC_H
#define _MTK_CEC_H

#include <linux/types.h>

struct device;

void mtk_cec_set_hpd_event(struct device *dev,
			   void (*hotplug_event)(bool hpd, struct device *dev),
			   struct device *hdmi_dev);
bool mtk_cec_hpd_high(struct device *dev);

#endif /* _MTK_CEC_H */
