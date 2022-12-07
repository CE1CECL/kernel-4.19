/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _GZ_TRACE_BUILTIN_H_
#define _GZ_TRACE_BUILTIN_H_

#if IS_ENABLED(CONFIG_FTRACE)

#include <linux/kallsyms.h>
#include <linux/trace_events.h>
static unsigned long __read_mostly gz_trusty_tracing_writer;

#define GZ_TRUSTY_TRACE_BEGIN(fmt, args...) do { \
	if (gz_trusty_tracing_writer == 0) \
		gz_trusty_tracing_writer = \
			kallsyms_lookup_name("tracing_mark_write"); \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, \
		"B|%d|GZT_"fmt"\n", current->tgid, ##args); \
	preempt_enable();\
} while (0)

#define GZ_TRUSTY_TRACE_END() do { \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, "E\n"); \
	preempt_enable(); \
} while (0)

#define GZ_TRUSTY_TRACE_CNT(cnt, fmt, args...) do { \
	if (gz_trusty_tracing_writer == 0) \
		gz_trusty_tracing_writer = \
			kallsyms_lookup_name("tracing_mark_write"); \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, \
		"C|%d|GZT_"fmt"|%d\n", current->tgid, ##args, cnt); \
	preempt_enable();\
} while (0)

#define GZ_TRUSTY_ASYNC_TRACE_BEGIN(cookie, fmt, args...) do { \
	if (gz_trusty_tracing_writer == 0) \
		gz_trusty_tracing_writer = \
			kallsyms_lookup_name("tracing_mark_write"); \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, \
		"S|%d|GZT_"fmt"|%d\n", current->tgid, ##args, cookie); \
	preempt_enable();\
} while (0)

#define GZ_TRUSTY_ASYNC_TRACE_END(cookie, fmt, args...) do { \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, \
		"F|%d|GZT_"fmt"|%d\n", current->tgid, ##args, cookie); \
	preempt_enable();\
} while (0)

#define GZ_TRUSTY_TRACE_INJECTION(fmt, args...) do { \
	if (gz_trusty_tracing_writer == 0) \
		gz_trusty_tracing_writer = \
			kallsyms_lookup_name("tracing_mark_write"); \
	preempt_disable(); \
	event_trace_printk(gz_trusty_tracing_writer, \
		"GZT|"fmt"\n", ##args); \
	preempt_enable();\
} while (0)

#else

#define GZ_TRUSTY_TRACE_BEGIN(fmt, args...) do { \
} while (0)

#define GZ_TRUSTY_TRACE_END() do { \
} while (0)

#define GZ_TRUSTY_TRACE_CNT(cnt, fmt, args...) do { \
} while (0)

#define GZ_TRUSTY_ASYNC_TRACE_BEGIN(cookie, fmt, args...) do { \
} while (0)

#define GZ_TRUSTY_ASYNC_TRACE_END(cookie, fmt, args...) do { \
} while (0)

#endif /* CONFIG_FTRACE */


#endif
