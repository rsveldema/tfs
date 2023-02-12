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
    .fs_flags = FS_REQUIRES_DEV,
};

static inline struct tfs_fs_info *tfs_sb(struct super_block *sb)
{
        return sb->s_fs_info;
}

static int tfs_test_super(struct super_block *s, void *data)
{
	struct tfs_fs_info *p = data;
	struct tfs_fs_info *fs_info = tfs_sb(s);

	return p->magic == fs_info->magic;
}

static int tfs_set_super(struct super_block *s, void *data)
{
	int err = set_anon_super(s, data);
	if (!err)
		s->s_fs_info = data;
	return err;
}


static struct dentry *tfs_mount_root(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data)
{
    char *orig_data = kstrdup(data, GFP_KERNEL);
    struct tfs_fs_info *fs_info = kzalloc(sizeof(*fs_info), GFP_KERNEL);
	struct super_block *s;

    fs_info->magic = 0xa115e;

    s = sget(fs_type, tfs_test_super, tfs_set_super, flags | SB_NOSEC,
                fs_info);
    if (IS_ERR(s)) {
        pr_err("failed to get super block\n");
    }

    kfree(orig_data);
    return NULL;
}

static struct dentry *tfs_mount(struct file_system_type *fs_type, int flags,
                                const char *dev_name, void *data)
{
    struct vfsmount *mnt_root;
    struct dentry *root;
    const char* subvol_name = "subvol";

    pr_err("tfs-mount %s\n", dev_name);
    mnt_root = vfs_kern_mount(&tfs_root_fs_type, flags, dev_name, data);
    if (PTR_ERR_OR_ZERO(mnt_root) == -EBUSY)
    {
        pr_err("mnt-root = busy?\n");
    }
    else
    {
        pr_err("mnt-root = OK\n");
    }

    if (IS_ERR(mnt_root))
    {
        pr_err("mnt-root has error?\n");
    }

    root = mount_subtree(mnt_root, subvol_name);
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
