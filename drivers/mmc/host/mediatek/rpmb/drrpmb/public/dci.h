/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __DCI_H__
#define __DCI_H__


typedef uint32_t dciCommandId_t;
typedef uint32_t dciResponseId_t;
typedef uint32_t dciReturnCode_t;

/**< Responses have bit 31 set */
#define RSP_ID_MASK (1U << 31)
#define RSP_ID(cmdId) (((uint32_t)(cmdId)) | RSP_ID_MASK)
#define IS_CMD(cmdId) ((((uint32_t)(cmdId)) & RSP_ID_MASK) == 0)
#define IS_RSP(cmdId) ((((uint32_t)(cmdId)) & RSP_ID_MASK) == RSP_ID_MASK)

#define RET_OK                    0
#define RET_ERR_UNKNOWN_CMD       1
#define RET_ERR_NOT_SUPPORTED     2
#define RET_ERR_INTERNAL_ERROR    3
/* ... add more error codes when needed */

struct dciCommandHeader_t {
	dciCommandId_t commandId; /**< Command ID */
};

struct dciResponseHeader_t {
	/**< Response ID (must be command ID | RSP_ID_MASK )*/
	dciResponseId_t     responseId;
	dciReturnCode_t     returnCode; /**< Return code of command */
};

#endif
