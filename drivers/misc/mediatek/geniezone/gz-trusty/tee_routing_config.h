/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _TEE_ROUTING_CONFIG_H_
#define _TEE_ROUTING_CONFIG_H_

#include <linux/types.h>
#include <gz-trusty/trusty.h>

#define MAX_TEE_ROUTING_SRV_NAME (16)
#define MAX_TEE_ROUTING_NUM (32)


struct tee_routing_obj {
	/**
	 * @srv_name
	 * e.g.  Echo-server: "com.mediatek.geniezone.srv.echo"
	 *	 The first word is "com".
	 *	 New-server: "nebula.com.mediatek.geniezone.srv.new"
	 *	 The first word is "nebula".
	 */
	char srv_name[MAX_TEE_ROUTING_SRV_NAME];
	enum tee_id_t tee_id;
	struct hlist_node node;
};

static struct tee_routing_obj tee_routing_config[MAX_TEE_ROUTING_NUM] = {
	/* The first element is the default. Do not modify it unless needed.*/
	[0] = { .srv_name = "com",
		.tee_id = TEE_ID_TRUSTY
	},

	{	.srv_name = "nebula",
		.tee_id = TEE_ID_NEBULA
	},

	{	.srv_name = "trusty",
		.tee_id = TEE_ID_TRUSTY
	},
	/* If the service only work for Trusty.
	 * {	.srv_name = "trusty_service",
	 *	.tee_id   = TEE_ID_TRUSTY
	 * },
	 */

	/* Keep the none as the last element. Do not modify it unless needed.*/
	{	.srv_name = "none",
		.tee_id = TEE_ID_END
	}
};

#endif
