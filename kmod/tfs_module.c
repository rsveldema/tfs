/*
 * Linux kernel module
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>


void *tfs_alloc(u32 size)
{
    return kmalloc(size, GFP_USER);
}

void tfs_free(void* ptr)
{
    kfree(ptr);
}


void tfs_panic(const char* msg)
{
    pr_err("PANIC: %s\n", msg);
    while (1) {}
}

static int __init module_load(void)
{
    pr_err("Loading C++ kernel module\n");

    return 0;
}

static void __exit module_unload(void)
{
    pr_err("Unloading C++ kernel module\n");
}

module_init(module_load);
module_exit(module_unload);

MODULE_DESCRIPTION ("TFS Linux kernel module");
MODULE_VERSION ("0.1");
MODULE_AUTHOR ("Ronald Veldema");
MODULE_LICENSE ("GPL");
