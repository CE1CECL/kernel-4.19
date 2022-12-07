/* SPDX-License-Identifier: GPL-2.0 */



#ifndef __GZ_MAIN_H
#define __GZ_MAIN_H

/* GP memory parameter max len. Needs to be synced with GZ */
#define GP_MEM_MAX_LEN 1024

#define KREE_SESSION_HANDLE_MAX_SIZE 512

static int __init gz_init(void);
static void __exit gz_exit(void);

static int gz_dev_open(struct inode *inode, struct file *filp);
static int gz_dev_release(struct inode *inode, struct file *filp);

static long gz_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
#if IS_ENABLED(CONFIG_COMPAT)
static long gz_compat_ioctl(struct file *filep, unsigned int cmd,
			    unsigned long arg);
#endif

#endif /* __GZ_MAIN_H */

