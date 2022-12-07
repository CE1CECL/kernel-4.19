/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __CMDQ_RECORD_H__
#define __CMDQ_RECORD_H__

#include <linux/types.h>
#include <linux/soc/mediatek/mtk-cmdq.h>

#include "cmdq_def.h"
#include "cmdq_helper_ext.h"

enum CMDQ_STACK_TYPE_ENUM {
	CMDQ_STACK_NULL = -1,
	CMDQ_STACK_TYPE_IF = 0,
	CMDQ_STACK_TYPE_ELSE = 1,
	CMDQ_STACK_TYPE_WHILE = 2,
	CMDQ_STACK_TYPE_BREAK = 3,
	CMDQ_STACK_TYPE_CONTINUE = 4,
	CMDQ_STACK_TYPE_DO_WHILE = 5,
};

#define CMDQ_DATA_BIT			(62)
#define CMDQ_BIT_VALUE			(0LL)
#define CMDQ_BIT_VAR			(1LL)
#define CMDQ_TASK_CPR_INITIAL_VALUE	(0)
#define CMDQ_REC_DEFAULT_PRIORITY	(100)
#define CMDQ_REC_MAX_PRIORITY		(0x7FFFFFFF)

struct cmdq_stack_node {
	u32 position;
	enum CMDQ_STACK_TYPE_ENUM stack_type;
	struct cmdq_stack_node *next;
};

struct cmdq_user_req {
	u32 reg_count;
	u32 user_token;
};

#define cmdqBackupSlotHandle dma_addr_t

s32 cmdq_task_create(enum CMDQ_SCENARIO_ENUM scenario,
	struct cmdqRecStruct **pHandle);
s32 cmdqRecCreate(enum CMDQ_SCENARIO_ENUM scenario,
	struct cmdqRecStruct **pHandle);
s32 cmdq_task_duplicate(struct cmdqRecStruct *handle,
	struct cmdqRecStruct **handle_out);

s32 cmdq_task_set_engine(struct cmdqRecStruct *handle,
	u64 engineFlag);
s32 cmdqRecSetEngine(struct cmdqRecStruct *handle,
	u64 engineFlag);

s32 cmdq_task_reset(struct cmdqRecStruct *handle);
s32 cmdqRecReset(struct cmdqRecStruct *handle);

void cmdq_task_set_timeout(struct cmdqRecStruct *handle, u32 timeout);

s32 cmdq_task_set_secure(struct cmdqRecStruct *handle, const bool is_secure);
s32 cmdqRecSetSecure(struct cmdqRecStruct *handle, const bool is_secure);

s32 cmdq_task_is_secure(struct cmdqRecStruct *handle);
s32 cmdqRecIsSecure(struct cmdqRecStruct *handle);

s32 cmdq_task_secure_enable_dapc(struct cmdqRecStruct *handle,
	const u64 engineFlag);
s32 cmdqRecSecureEnableDAPC(struct cmdqRecStruct *handle,
	const u64 engineFlag);

s32 cmdq_task_secure_enable_port_security(struct cmdqRecStruct *handle,
	const u64 engineFlag);
s32 cmdqRecSecureEnablePortSecurity(struct cmdqRecStruct *handle,
	const u64 engineFlag);

/* Assign secure metadata for client driver */
s32 cmdq_task_set_secure_meta(struct cmdqRecStruct *handle,
	enum cmdq_sec_rec_meta_type type, void *meta, u32 size);

s32 cmdqRecMark(struct cmdqRecStruct *handle);

s32 cmdqRecEnablePrefetch(struct cmdqRecStruct *handle);

s32 cmdqRecDisablePrefetch(struct cmdqRecStruct *handle);

s32 cmdq_op_write_reg(struct cmdqRecStruct *handle, u32 addr,
	CMDQ_VARIABLE argument, u32 mask);
s32 cmdqRecWrite(struct cmdqRecStruct *handle, u32 addr, u32 value, u32 mask);

void cmdq_task_set_mtee(struct cmdqRecStruct *handle, const bool enable);
s32 cmdq_op_write_reg_secure(struct cmdqRecStruct *handle, u32 addr,
	enum CMDQ_SEC_ADDR_METADATA_TYPE type, u64 baseHandle,
	u32 offset, u32 size, u32 port);
s32 cmdqRecWriteSecure(struct cmdqRecStruct *handle,
	u32 addr, enum CMDQ_SEC_ADDR_METADATA_TYPE type,
	u64 baseHandle, u32 offset, u32 size, u32 port);

s32 cmdq_op_poll(struct cmdqRecStruct *handle, u32 addr, u32 value, u32 mask);
s32 cmdqRecPoll(struct cmdqRecStruct *handle, u32 addr, u32 value, u32 mask);

s32 cmdq_op_wait(struct cmdqRecStruct *handle, enum cmdq_event event);
s32 cmdqRecWait(struct cmdqRecStruct *handle, enum cmdq_event event);

s32 cmdq_op_wait_no_clear(struct cmdqRecStruct *handle,
	enum cmdq_event event);
s32 cmdqRecWaitNoClear(struct cmdqRecStruct *handle,
	enum cmdq_event event);

s32 cmdq_op_clear_event(struct cmdqRecStruct *handle,
	enum cmdq_event event);
s32 cmdqRecClearEventToken(struct cmdqRecStruct *handle,
	enum cmdq_event event);

s32 cmdq_op_set_event(struct cmdqRecStruct *handle,
	enum cmdq_event event);
s32 cmdqRecSetEventToken(struct cmdqRecStruct *handle,
	enum cmdq_event event);

s32 cmdq_op_replace_overwrite_cpr(struct cmdqRecStruct *handle, u32 index,
	s32 new_arg_a, s32 new_arg_b, s32 new_arg_c);

s32 cmdq_op_read_to_data_register(struct cmdqRecStruct *handle,
	u32 hw_addr, enum cmdq_gpr_reg dst_data_reg);
s32 cmdqRecReadToDataRegister(struct cmdqRecStruct *handle, u32 hw_addr,
	enum cmdq_gpr_reg dst_data_reg);

s32 cmdq_op_write_from_data_register(struct cmdqRecStruct *handle,
	enum cmdq_gpr_reg src_data_reg, u32 hw_addr);
s32 cmdqRecWriteFromDataRegister(struct cmdqRecStruct *handle,
	enum cmdq_gpr_reg src_data_reg, u32 hw_addr);

struct cmdq_command_buffer {
	void *va_base;
	u32 cmd_buf_size;
	u32 avail_buf_size;
};
s32 cmdq_op_poll_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, u32 addr,
	CMDQ_VARIABLE value, u32 mask);
s32 cmdq_op_read_reg_to_mem_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 addr);
s32 cmdq_op_write_reg_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, u32 addr,
	CMDQ_VARIABLE value, u32 mask);
s32 cmdq_op_wait_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, enum cmdq_event event);
s32 cmdq_op_wait_no_clear_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, enum cmdq_event event);
s32 cmdq_op_clear_event_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, enum cmdq_event event);
s32 cmdq_op_set_event_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, enum cmdq_event event);
s32 cmdq_op_acquire_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, enum cmdq_event event);
s32 cmdq_op_write_from_reg_ex(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf, u32 write_reg, u32 from_reg);
s32 cmdq_handle_flush_cmd_buf(struct cmdqRecStruct *handle,
	struct cmdq_command_buffer *cmd_buf);
s32 cmdq_alloc_write_addr(u32 count, dma_addr_t *paStart, u32 clt, void *fp);
s32 cmdq_free_write_addr(dma_addr_t paStart, u32 clt);
s32 cmdq_free_write_addr_by_node(u32 clt, void *fp);

s32 cmdq_alloc_mem(cmdqBackupSlotHandle *p_h_backup_slot, u32 slotCount);
s32 cmdqBackupAllocateSlot(cmdqBackupSlotHandle *p_h_backup_slot,
	u32 slotCount);

s32 cmdq_cpu_read_mem(cmdqBackupSlotHandle h_backup_slot, u32 slot_index,
	u32 *value);
s32 cmdqBackupReadSlot(cmdqBackupSlotHandle h_backup_slot, u32 slot_index,
	u32 *value);

s32 cmdq_cpu_write_mem(cmdqBackupSlotHandle h_backup_slot,
	u32 slot_index, u32 value);
s32 cmdqBackupWriteSlot(cmdqBackupSlotHandle h_backup_slot, u32 slot_index,
	u32 value);


s32 cmdq_free_mem(cmdqBackupSlotHandle h_backup_slot);
s32 cmdqBackupFreeSlot(cmdqBackupSlotHandle h_backup_slot);


s32 cmdq_op_read_reg_to_mem(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 addr);
s32 cmdqRecBackupRegisterToSlot(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 addr);

s32 cmdq_op_read_mem_to_reg(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 addr);
s32 cmdqRecBackupWriteRegisterFromSlot(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 addr);

s32 cmdq_op_write_mem(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 value);
s32 cmdqRecBackupUpdateSlot(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index, u32 value);

s32 cmdq_op_finalize_command(struct cmdqRecStruct *handle, bool loop);

void cmdq_task_prepare(struct cmdqRecStruct *handle);
void cmdq_task_unprepare(struct cmdqRecStruct *handle);

s32 cmdq_task_update_property(struct cmdqRecStruct *handle,
	void *prop_addr, u32 prop_size);

s32 cmdq_task_flush(struct cmdqRecStruct *handle);
s32 cmdqRecFlush(struct cmdqRecStruct *handle);

s32 cmdq_task_append_backup_reg(struct cmdqRecStruct *handle,
	u32 reg_count, u32 *addrs);
s32 cmdq_task_flush_and_read_register(struct cmdqRecStruct *handle,
	u32 reg_count, u32 *addrs, u32 *values_out);
s32 cmdqRecFlushAndReadRegister(struct cmdqRecStruct *handle,
	u32 reg_count, u32 *addrs, u32 *values_out);

s32 cmdq_task_flush_async(struct cmdqRecStruct *handle);
s32 cmdqRecFlushAsync(struct cmdqRecStruct *handle);

s32 cmdq_task_flush_async_callback(struct cmdqRecStruct *handle,
	CmdqAsyncFlushCB callback, u64 user_data);
s32 cmdq_task_flush_async_destroy(struct cmdqRecStruct *handle);
s32 cmdqRecFlushAsyncCallback(struct cmdqRecStruct *handle,
	CmdqAsyncFlushCB callback, u64 user_data);

s32 cmdq_task_start_loop(struct cmdqRecStruct *handle);
s32 cmdqRecStartLoop(struct cmdqRecStruct *handle);

s32 cmdq_task_start_loop_callback(struct cmdqRecStruct *handle,
	CmdqInterruptCB loopCB, unsigned long loopData);
s32 cmdqRecStartLoopWithCallback(struct cmdqRecStruct *handle,
	CmdqInterruptCB loopCB, unsigned long loopData);

s32 cmdq_task_start_loop_sram(struct cmdqRecStruct *handle,
	const char *SRAM_owner_name);

s32 cmdq_task_stop_loop(struct cmdqRecStruct *handle);
s32 cmdqRecStopLoop(struct cmdqRecStruct *handle);

s32 cmdq_task_copy_to_sram(dma_addr_t pa_src, u32 sram_dest, size_t size);
s32 cmdq_task_copy_from_sram(dma_addr_t pa_dest, u32 sram_src, size_t size);

s32 cmdq_task_get_inst_cnt(struct cmdqRecStruct *handle);
s32 cmdqRecGetInstructionCount(struct cmdqRecStruct *handle);

s32 cmdq_op_profile_marker(struct cmdqRecStruct *handle, const char *tag);
s32 cmdqRecProfileMarker(struct cmdqRecStruct *handle, const char *tag);

s32 cmdqRecDumpCommand(struct cmdqRecStruct *handle);

s32 cmdq_task_destroy(struct cmdqRecStruct *handle);
void cmdqRecDestroy(struct cmdqRecStruct *handle);

s32 cmdq_op_set_nop(struct cmdqRecStruct *handle, u32 index);
s32 cmdqRecSetNOP(struct cmdqRecStruct *handle, u32 index);

s32 cmdq_task_query_offset(struct cmdqRecStruct *handle, u32 startIndex,
	const enum cmdq_code opCode, enum cmdq_event event);
s32 cmdqRecQueryOffset(struct cmdqRecStruct *handle, u32 startIndex,
	const enum cmdq_code opCode, enum cmdq_event event);

s32 cmdq_resource_acquire(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent);
s32 cmdqRecAcquireResource(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent);

s32 cmdq_resource_acquire_and_write(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent, u32 addr, u32 value, u32 mask);
s32 cmdqRecWriteForResource(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent, u32 addr, u32 value, u32 mask);

s32 cmdq_resource_release(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent);
s32 cmdqRecReleaseResource(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent);

s32 cmdq_resource_release_and_write(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent, u32 addr, u32 value, u32 mask);
s32 cmdqRecWriteAndReleaseResource(struct cmdqRecStruct *handle,
	enum cmdq_event resourceEvent, u32 addr, u32 value, u32 mask);

s32 cmdq_task_create_delay_thread_dram(void **pp_delay_thread_buffer,
	u32 *buffer_size);
s32 cmdq_task_create_delay_thread_sram(void **pp_delay_thread_buffer,
	u32 *buffer_size, u32 *cpr_offset);

/* tablet use */
#ifdef CONFIG_MTK_IN_HOUSE_TEE_SUPPORT
	s32 cmdq_task_set_secure_mode(struct cmdqRecStruct *handle,
		enum CMDQ_DISP_MODE mode);
	s32 cmdqRecSetSecureMode(struct cmdqRecStruct *handle,
		enum CMDQ_DISP_MODE mode);
#endif

void cmdq_op_init_variable(CMDQ_VARIABLE *arg);

void cmdq_op_init_global_cpr_variable(CMDQ_VARIABLE *arg, u32 cpr_offset);

s32 cmdq_op_assign(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_in);

s32 cmdq_op_add(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_subtract(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_multiply(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_xor(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_not(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b);

s32 cmdq_op_or(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_and(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_left_shift(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_right_shift(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
	CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

s32 cmdq_op_delay_us(struct cmdqRecStruct *handle, u32 delay_time);
s32 cmdq_op_backup_CPR(struct cmdqRecStruct *handle, CMDQ_VARIABLE cpr,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index);
s32 cmdq_op_backup_TPR(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index);

s32 cmdq_op_if(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
	enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

s32 cmdq_op_end_if(struct cmdqRecStruct *handle);

s32 cmdq_op_else(struct cmdqRecStruct *handle);

s32 cmdq_op_else_if(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
	enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

s32 cmdq_op_while(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
	enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

s32 cmdq_op_continue(struct cmdqRecStruct *handle);

s32 cmdq_op_break(struct cmdqRecStruct *handle);

s32 cmdq_op_end_while(struct cmdqRecStruct *handle);

s32 cmdq_op_do_while(struct cmdqRecStruct *handle);

s32 cmdq_op_end_do_while(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
	enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

s32 cmdq_op_wait_event_timeout(struct cmdqRecStruct *handle,
	CMDQ_VARIABLE *arg_out, enum cmdq_event wait_event,
	u32 timeout_time);

s32 cmdq_op_read_reg(struct cmdqRecStruct *handle, u32 addr,
	CMDQ_VARIABLE *arg_out, u32 mask);

s32 cmdq_op_read_mem(struct cmdqRecStruct *handle,
	cmdqBackupSlotHandle h_backup_slot, u32 slot_index,
	CMDQ_VARIABLE *arg_out);

#endif	/* __CMDQ_RECORD_H__ */
