/*
 * Linux kernel module
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/dax.h>

static struct dentry *tfs_mount(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data);

static struct dentry *tfs_mount_root(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data);

void *tfs_alloc(u32 size)
{
    return kmalloc(size, GFP_KERNEL);
}

void tfs_free(void *ptr)
{
    kfree(ptr);
}

void tfs_panic(const char *msg)
{
    pr_err("PANIC: %s\n", msg);
    panic(KERN_CRIT "TFS PANIC: %s\n", msg);
    BUG();
    while (1)
    {
    }
}

struct tfs_fs_info
{
    struct super_block *s_sb;
    u32 magic;
};

static struct file_system_type tfs_fs_type = {
    .owner = THIS_MODULE,
    .name = "tfs",
    .mount = tfs_mount,
    .kill_sb = kill_block_super,
    .fs_flags = FS_REQUIRES_DEV,
};

static struct file_system_type tfs_root_fs_type = {
    .owner = THIS_MODULE,
    .name = "tfs",
    .mount = tfs_mount_root,
    .kill_sb = kill_block_super,
    .fs_flags = FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
};

static inline struct tfs_fs_info *tfs_sb(struct super_block *sb)
{
    if (sb == NULL) {
        pr_err("null passed to tfs_sb\n");
        return NULL;
    }
    return sb->s_fs_info;
}

static int tfs_test_super(struct super_block *s, void *data)
{
	struct tfs_fs_info *p = data;
	struct tfs_fs_info *fs_info = tfs_sb(s);

    if (! fs_info || ! p)
    {
        return 0;
    }

    pr_err("tfs-test-super: %p vs %p\n", p, fs_info);

	return p->magic == fs_info->magic;
}

static int tfs_set_super(struct super_block *s, void *data)
{
    int err;
    pr_err("tfs-set-super: %p vs %p\n", s, data);

	err = set_anon_super(s, data);
	if (!err)
		s->s_fs_info = data;
    pr_err("tfs-set-super2: %p vs %p\n", s, data);
	return err;
}


#define TFS_FIRST_INODE_ID      256


struct tfs_key
{
    u64 inode_num;
};

typedef struct tfs_key tfs_iget_args;

struct tfs_inode
{
	struct inode vfs_inode;
	struct tfs_key location;
};

static inline struct tfs_inode *TFS_I(const struct inode *inode)
{
	return container_of(inode, struct tfs_inode, vfs_inode);
}



static int tfs_find_actor(struct inode *inode, void *opaque)
{
	tfs_iget_args *args = opaque;

	return args->inode_num == TFS_I(inode)->location.inode_num; // &&
		//args->root == TFS_I(inode)->root;
}

static u64 tfs_inode_hash(u64 inode_num)
{
    return inode_num;
}

static int tfs_init_locked_inode(struct inode *inode, void *opaque)
{
	tfs_iget_args *args = opaque;

	inode->i_ino = args->inode_num;
    return 0;
}


struct inode* tfs_iget(struct super_block *s, struct tfs_fs_info *fs_info, u64 inode_num)
{
	tfs_iget_args args;
	struct inode *inode;
	unsigned long hashval = tfs_inode_hash(inode_num);

    args.inode_num = inode_num;

	inode = iget5_locked(s, hashval, tfs_find_actor,
			     tfs_init_locked_inode,
			     (void *)&args);
	return inode;
}


static struct dentry *tfs_mount_root(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data)
{
    char *orig_data = kstrdup(data, GFP_KERNEL);
    struct tfs_fs_info *fs_info = kzalloc(sizeof(*fs_info), GFP_KERNEL);
	struct super_block *s;
    struct inode *inode;
    struct dentry *root_dir = NULL;

    fs_info->magic = 0xa115e;

    pr_err("trying tfs-mount-root: %p\n", fs_info);

    s = sget(fs_type, tfs_test_super, tfs_set_super,
            flags | SB_NOSEC, fs_info);
    if (IS_ERR(s)) {
        pr_err("failed to get super block\n");
    }

    kfree(orig_data);
 
    pr_err("trying tfs-mount-root1 %p\n", s);
    if (! s)
    {
        return ERR_PTR(-EINVAL);
    }

    inode = tfs_iget(s, fs_info, TFS_FIRST_INODE_ID);

    pr_err("trying tfs-mount-root-inode %p\n", inode);

    s->s_root = d_make_root(inode);
	s->s_flags |= SB_ACTIVE;

    pr_err("trying tfs-mount-root2 %p\n", s->s_root);

    root_dir = dget(s->s_root);
    pr_err("trying tfs-mount-root3 %p\n", root_dir);
    return root_dir;
}

static struct dentry *tfs_mount(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data)
{
    struct vfsmount *mnt_root;
    struct dentry *root;
    char* subvol_name;

    if (data == NULL)
    {
        pr_err("SKIP tfs-mount %s, data %p\n", dev_name, data);
        return ERR_PTR(-EINVAL);
    }

    subvol_name = kzalloc(32, GFP_KERNEL);
    strcpy(subvol_name, "subvol");

    pr_err("tfs-mount %s, datea %p\n", dev_name, data);
    mnt_root = vfs_kern_mount(&tfs_root_fs_type, flags | SB_NOSEC,
                    dev_name, data);
    if (PTR_ERR_OR_ZERO(mnt_root) == -EBUSY)
    {
        pr_err("mnt-root = busy?\n");
        return ERR_PTR(-EINVAL);
    }
    else
    {
        pr_err("mnt-root is not busy: %p\n", mnt_root);
    }

    if (IS_ERR(mnt_root))
    {
        pr_err("mnt-root has error - %p\n", mnt_root);
        return ERR_PTR(-EINVAL);
    }

    root = mount_subtree(mnt_root, subvol_name);
    pr_err("mnt-root-subtree ret %p\n", root);
    if (IS_ERR(root))
    {
        may_umount(mnt_root);
		//deactivate_locked_super(s);
        return ERR_PTR(-EINVAL);
    }
    return root;
}

MODULE_ALIAS_FS("tfs");

static int __init module_load(void)
{
    int err = 0;
    pr_err("Loading TFS\n");
    err = register_filesystem(&tfs_fs_type);
    return 0;
}

static void __exit module_unload(void)
{
    pr_err("Unloading TFS\n");
    unregister_filesystem(&tfs_fs_type);
}

module_init(module_load);
module_exit(module_unload);

MODULE_DESCRIPTION("TFS Linux kernel module");
MODULE_VERSION("0.1");
MODULE_AUTHOR("Ronald Veldema");
MODULE_LICENSE("GPL");
