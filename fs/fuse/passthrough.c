// SPDX-License-Identifier: GPL-2.0

#include <linux/file.h>
#include "fuse_i.h"

#include <linux/uio.h>

struct fuse_aio_req {
	struct kiocb iocb;
	struct kiocb *iocb_fuse;
};

static void fuse_copyattr(struct file *dst_file, struct file *src_file)
{
	struct inode *dst = file_inode(dst_file);
	struct inode *src = file_inode(src_file);

	i_size_write(dst, i_size_read(src));
}

static rwf_t iocbflags_to_rwf(int ifl)
{
	rwf_t flags = 0;

	if (ifl & IOCB_APPEND)
		flags |= RWF_APPEND;
	if (ifl & IOCB_DSYNC)
		flags |= RWF_DSYNC;
	if (ifl & IOCB_HIPRI)
		flags |= RWF_HIPRI;
	if (ifl & IOCB_NOWAIT)
		flags |= RWF_NOWAIT;
	if (ifl & IOCB_SYNC)
		flags |= RWF_SYNC;

	return flags;
}

static const struct cred *
fuse_passthrough_override_creds(const struct file *fuse_filp)
{
	struct inode *fuse_inode = file_inode(fuse_filp);
	struct fuse_conn *fc = fuse_inode->i_sb->s_fs_info;

	return override_creds(fc->creator_cred);
}

static void fuse_aio_cleanup_handler(struct fuse_aio_req *aio_req)
{
	struct kiocb *iocb = &aio_req->iocb;
	struct kiocb *iocb_fuse = aio_req->iocb_fuse;

	if (iocb->ki_flags & IOCB_WRITE) {
		__sb_writers_acquired(file_inode(iocb->ki_filp)->i_sb,
				      SB_FREEZE_WRITE);
		file_end_write(iocb->ki_filp);
		fuse_copyattr(iocb_fuse->ki_filp, iocb->ki_filp);
	}

	iocb_fuse->ki_pos = iocb->ki_pos;
	kfree(aio_req);
}

static void fuse_aio_rw_complete(struct kiocb *iocb, long res, long res2)
{
	struct fuse_aio_req *aio_req =
		container_of(iocb, struct fuse_aio_req, iocb);
	struct kiocb *iocb_fuse = aio_req->iocb_fuse;

	fuse_aio_cleanup_handler(aio_req);
	iocb_fuse->ki_complete(iocb_fuse, res, res2);
}

ssize_t fuse_passthrough_read_iter(struct kiocb *iocb_fuse,
				   struct iov_iter *iter)
{
	ssize_t ret;
	const struct cred *old_cred;
	struct file *fuse_filp = iocb_fuse->ki_filp;
	struct fuse_file *ff = fuse_filp->private_data;
	struct file *passthrough_filp = ff->passthrough_filp;

	if (!iov_iter_count(iter))
		return 0;

	old_cred = fuse_passthrough_override_creds(fuse_filp);
	if (is_sync_kiocb(iocb_fuse)) {
		ret = vfs_iter_read(passthrough_filp, iter, &iocb_fuse->ki_pos,
				    iocbflags_to_rwf(iocb_fuse->ki_flags));
	} else {
		struct fuse_aio_req *aio_req;

		aio_req = kmalloc(sizeof(struct fuse_aio_req), GFP_KERNEL);
		if (!aio_req)
			return -ENOMEM;

		aio_req->iocb_fuse = iocb_fuse;
		kiocb_clone(&aio_req->iocb, iocb_fuse, passthrough_filp);
		aio_req->iocb.ki_complete = fuse_aio_rw_complete;
		ret = call_read_iter(passthrough_filp, &aio_req->iocb, iter);
		if (ret != -EIOCBQUEUED)
			fuse_aio_cleanup_handler(aio_req);
	}
	revert_creds(old_cred);

	return ret;
}

ssize_t fuse_passthrough_write_iter(struct kiocb *iocb_fuse,
				    struct iov_iter *iter)
{
	ssize_t ret;
	const struct cred *old_cred;
	struct file *fuse_filp = iocb_fuse->ki_filp;
	struct fuse_file *ff = fuse_filp->private_data;
	struct inode *fuse_inode = file_inode(fuse_filp);
	struct file *passthrough_filp = ff->passthrough_filp;
	struct inode *passthrough_inode = file_inode(passthrough_filp);

	if (!iov_iter_count(iter))
		return 0;

	inode_lock(fuse_inode);

	old_cred = fuse_passthrough_override_creds(fuse_filp);
	if (is_sync_kiocb(iocb_fuse)) {
		file_start_write(passthrough_filp);
		ret = vfs_iter_write(passthrough_filp, iter, &iocb_fuse->ki_pos,
				    iocbflags_to_rwf(iocb_fuse->ki_flags));
		file_end_write(passthrough_filp);
		if (ret > 0)
			fuse_copyattr(fuse_filp, passthrough_filp);
	} else {
		struct fuse_aio_req *aio_req;

		aio_req = kmalloc(sizeof(struct fuse_aio_req), GFP_KERNEL);
		if (!aio_req) {
			ret = -ENOMEM;
			goto out;
		}

		file_start_write(passthrough_filp);
		__sb_writers_release(passthrough_inode->i_sb, SB_FREEZE_WRITE);

		aio_req->iocb_fuse = iocb_fuse;
		kiocb_clone(&aio_req->iocb, iocb_fuse, passthrough_filp);
		aio_req->iocb.ki_complete = fuse_aio_rw_complete;
		ret = call_write_iter(passthrough_filp, &aio_req->iocb, iter);
		if (ret != -EIOCBQUEUED)
			fuse_aio_cleanup_handler(aio_req);
	}
out:
	revert_creds(old_cred);
	inode_unlock(fuse_inode);

	return ret;
}

int fuse_passthrough_setup(struct fuse_req *req, unsigned int fd)
{
	int ret;
	int fs_stack_depth;
	struct file *passthrough_filp;
	struct inode *passthrough_inode;
	struct super_block *passthrough_sb;

	/* Passthrough mode can only be enabled at file open/create time */
	if (req->in.h.opcode != FUSE_OPEN && req->in.h.opcode != FUSE_CREATE) {
		pr_err("FUSE: invalid OPCODE for request.\n");
		return -EINVAL;
	}

	passthrough_filp = fget(fd);
	if (!passthrough_filp) {
		pr_err("FUSE: invalid file descriptor for passthrough.\n");
		return -EINVAL;
	}

	ret = -EINVAL;
	if (!passthrough_filp->f_op->read_iter ||
	    !passthrough_filp->f_op->write_iter) {
		pr_err("FUSE: passthrough file misses file operations.\n");
		goto out;
	}

	passthrough_inode = file_inode(passthrough_filp);
	passthrough_sb = passthrough_inode->i_sb;
	fs_stack_depth = passthrough_sb->s_stack_depth + 1;
	ret = -EEXIST;
	if (fs_stack_depth > FILESYSTEM_MAX_STACK_DEPTH) {
		pr_err("FUSE: maximum fs stacking depth exceeded for passthrough\n");
		goto out;
	}

	req->passthrough_filp = passthrough_filp;
	return 0;
out:
	fput(passthrough_filp);
	return ret;
}

void fuse_passthrough_release(struct fuse_file *ff)
{
	if (!ff->passthrough_filp)
		return;

	fput(ff->passthrough_filp);
	ff->passthrough_filp = NULL;
}

int fuse_passthrough_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct fuse_file *ff = file->private_data;
	struct file *passthrough_filp = ff->passthrough_filp;
	const struct cred *old_cred;
	int ret;

	if (!passthrough_filp->f_op->mmap)
		return -ENODEV;

	if (WARN_ON(file != vma->vm_file))
		return -EIO;

	vma->vm_file = get_file(passthrough_filp);
	ret = call_mmap(vma->vm_file, vma);
	old_cred = fuse_passthrough_override_creds(file);
	revert_creds(old_cred);
	if (ret) {
		/* Drop reference count from new vm_file value */
		fput(passthrough_filp);
	} else {
		/* Drop reference count from previous vm_file value */
		fput(file);
	}
	file_accessed(file);

	return ret;
}

#if CONFIG_FUSE_PASSTHROUGH_WORKAROUND
static char *__dentry_name(struct dentry *dentry, char *name)
{
	char *p = dentry_path_raw(dentry, name, PATH_MAX);
	/*
	 * This function relies on the fact that dentry_path_raw() will place
	 * the path name at the end of the provided buffer.
	 */
	BUG_ON(p + strlen(p) + 1 != name + PATH_MAX);

	if (p > name)
		strcpy(name, p);

	return name;
}

char *dentry_name(struct dentry *dentry)
{
	char *name = __getname();
	if (!name)
		return NULL;

	return __dentry_name(dentry, name);
}

void fuse_validate_passthrough(struct fuse_req *req)
{
	char *fname;
	BUG_ON(!req->fname);
	BUG_ON(!req->passthrough_filp);

	fname = dentry_name(file_dentry(req->passthrough_filp));
	BUG_ON(!fname);
	if (strcasecmp(fname, req->fname)) {
		fput(req->passthrough_filp);
		req->passthrough_filp = NULL;
	}
	__putname(fname);
}
#endif
