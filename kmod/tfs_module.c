/*
 * Linux kernel module
 */

#include <linux/kernel.h>
#include <linux/module.h>

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
