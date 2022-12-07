/*
 * Copyright (C) 2020 Tcl Corporation Limited
 * Author: wu-yan@tcl.com
 * Date: 2020/08/26
 * This module is used to collect delay data like ui iowait
 * and export through procfs.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/proc_fs.h>
#include <linux/cpumask.h>
#include <linux/uaccess.h>
#include <linux/tct/stat.h>

#define HIST_SLOTS (12)
typedef struct {
	u64 min;
	u64 max;
	u64 sum;
	u64 count;
	u64 hist[HIST_SLOTS];
} tstat_t;

static inline void tstat_init(tstat_t * stat)
{
	memset(stat, 0, sizeof(*stat));
	stat->min = -1ULL;
}

static inline void tstat_add(tstat_t * stat, u64 value)
{
	int i;
	stat->sum += value;
	stat->min = min(stat->min, value);
	stat->max = max(stat->max, value);
	stat->count++;
	i = min(fls64(value >> 20), HIST_SLOTS - 1);
	stat->hist[i]++;
}

static inline void tstat_sum(tstat_t * dst, tstat_t * src)
{
	int i;
	if (!src->count)
		return;
	dst->sum += src->sum;
	dst->min = min(dst->min, src->min);
	dst->max = max(dst->max, src->max);
	dst->count += src->count;
	for (i = 0; i < HIST_SLOTS; i++) {
		dst->hist[i] += src->hist[i];
	}
}

struct tctstat_data {
	tstat_t __percpu *cpu_stat;
	struct proc_dir_entry *dir;
	bool enable;
};

static struct tctstat_data tsd;
/**
 * tctstat_account_delay -- record the statistics information of system
 * @value the value to record when event occur
 * @type  the type of event to record
 */
void tctstat_account_delay(u64 value, int type)
{
	tstat_t *stat;
	if (!tsd.enable)
		return;
	stat = &get_cpu_ptr(tsd.cpu_stat)[type];
	tstat_add(stat, value);
	put_cpu_ptr(tsd.cpu_stat);

}

EXPORT_SYMBOL(tctstat_account_delay);

/* /proc/tct_stat/iowait */

/*
 * Ease the printing of nsec fields:
 */
static long long nsec_high(unsigned long long nsec)
{
	if ((long long)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		return -nsec;
	}
	do_div(nsec, 1000000);

	return nsec;
}

static unsigned long nsec_low(unsigned long long nsec)
{
	if ((long long)nsec < 0)
		nsec = -nsec;

	return do_div(nsec, 1000000);
}

#define SPLIT_NS(x) nsec_high(x), nsec_low(x)
static int iowait_show(struct seq_file *m, void *v)
{
	int cpu, i;
	tstat_t stat, *cstat;
	tstat_init(&stat);
	preempt_disable();
	for_each_possible_cpu(cpu) {
		cstat = &per_cpu_ptr(tsd.cpu_stat, cpu)[UI_IOWAIT];
		tstat_sum(&stat, cstat);
	}
	preempt_enable();
	if (!stat.count)
		stat.min = 0;
	seq_printf(m, "min: %lld.%06ld\n", SPLIT_NS(stat.min));
	seq_printf(m, "max: %lld.%06ld\n", SPLIT_NS(stat.max));
	//seq_printf(m, "avg: %lld.%06ld\n", SPLIT_NS(stat.sum / stat.count));
	seq_printf(m, "sum: %lld.%06ld\n", SPLIT_NS(stat.sum));
	seq_printf(m, "count: %lld\n", stat.count);
	for (i = 0; i < HIST_SLOTS - 1; i++) {
		seq_printf(m, "<%dms: %u\n", 1 << i, stat.hist[i]);
	}
	seq_printf(m, ">=%dms: %u\n", 1 << (i - 1), stat.hist[i]);
	return 0;
}

static int iowait_open(struct inode *inode, struct file *file)
{
	return single_open(file, iowait_show, PDE_DATA(inode));
}

static ssize_t iowait_write(struct file *file, const char __user * buffer,
			    size_t count, loff_t * ppos)
{
	int argc, value, cpu;
	char input[4];
	bool enable;
	if (count >= sizeof(input))
		return -EINVAL;
	if (copy_from_user(input, buffer, count))
		return -EFAULT;
	input[count] = '\0';
	argc = sscanf(input, "%d", &value);
	switch (value) {
	case 0:		/* disable */
		tsd.enable = false;
		break;
	case 1:		/* enable */
		tsd.enable = true;
		break;
	case 2:		/* reset */
		enable = tsd.enable;
		tsd.enable = false;
		preempt_disable();
		for_each_possible_cpu(cpu) {
			tstat_t *stat = per_cpu_ptr(tsd.cpu_stat, cpu);
			int i;
			for (i = 0; i < NR_TCT_STAT; i++) {
				tstat_init(&stat[i]);
			}
		}
		preempt_enable();
		tsd.enable = enable;
		break;
	default:
		return -EINVAL;
	}
	return count;
}

static const struct file_operations iowait_fops = {
	.open = iowait_open,
	.read = seq_read,
	.write = iowait_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init tctstat_init(void)
{
	int cpu;
	tsd.cpu_stat = __alloc_percpu(NR_TCT_STAT * sizeof(tstat_t),
				      __alignof__(tstat_t));
	if (!tsd.cpu_stat) {
		pr_err("out of memory!\n");
		return -ENOMEM;
	}

	preempt_disable();
	for_each_possible_cpu(cpu) {
		tstat_t *stat = per_cpu_ptr(tsd.cpu_stat, cpu);
		int i;
		for (i = 0; i < NR_TCT_STAT; i++) {
			tstat_init(&stat[i]);
		}
	}
	preempt_enable();

	tsd.dir = proc_mkdir("tct_stat", NULL);
	if (!tsd.dir) {
		pr_err("failed to create tct_stat proc entry");
		free_percpu(tsd.cpu_stat);
		tsd.cpu_stat = NULL;
		return -ENOMEM;
	}
	proc_create_data("iowait", 0644, tsd.dir, &iowait_fops, NULL);
	pr_debug("enable tct statistics");
	tsd.enable = true;
	return 0;
}

static void __exit tctstat_exit(void)
{
	tsd.enable = false;
	if (tsd.cpu_stat) {
		remove_proc_subtree("tct_stat", NULL);
		free_percpu(tsd.cpu_stat);
		tsd.cpu_stat = NULL;
	}
}

module_init(tctstat_init);
module_exit(tctstat_exit);

MODULE_AUTHOR("yanwu <wu-yan@tcl.com>");
MODULE_DESCRIPTION("tct statistics driver");
MODULE_LICENSE("GPL");
