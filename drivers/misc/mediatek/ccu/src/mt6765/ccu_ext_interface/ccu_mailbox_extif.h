/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __CCU_MAILBOX_EXTIF__
#define __CCU_MAILBOX_EXTIF__

#include "ccu_ext_interface/ccu_ext_interface.h"

/*must be power of 2 for modulo operation take work*/
#define CCU_MAILBOX_QUEUE_SIZE 8

struct ccu_mailbox_t {
		MUINT32 front;
		MUINT32 rear;
		struct ccu_msg_t queue[CCU_MAILBOX_QUEUE_SIZE];
};

#endif
