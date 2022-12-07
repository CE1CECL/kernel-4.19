// SPDX-License-Identifier: GPL-2.0

#include <linux/device.h>       /* needed by device_* */
#include "scp_ipi.h"
#include "scp_scpctl.h"

static ssize_t scpctl_store(struct device *kobj
		, struct device_attribute *attr, const char *buf, size_t n)
{
	int ret;
	int magic, type, op;
	struct scpctl_cmd_s cmd;
	char *prompt = "[SCPCTL]:";

	if (sscanf(buf, "%d %d %d", &magic, &type, &op) != 3)
		return -EINVAL;
	pr_notice("%s %d %d %d\n", prompt, magic, type, op);

	if (magic != 666)
		return -EINVAL;

	cmd.type = type;
	cmd.op = op;

	switch (type) {
	case SCPCTL_TYPE_TMON:
		ret = scp_ipi_send(IPI_SCPCTL, &cmd, sizeof(cmd), 0, SCP_A_ID);
		if (ret != SCP_IPI_DONE)
			goto _err;
		break;
	default:
		return -EINVAL;
	}

	return n;

_err:
	pr_notice("%s failed, %d\n", prompt, ret);
	return -EIO;
}

DEVICE_ATTR_WO(scpctl);


