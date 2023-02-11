/*
 * Linux kernel module
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include "logger.h"
#include "cpp_module.h"

static int __init module_load(void)
{
    kern_log("Loading C++ kernel module\n");

    init_cpp_subsystem_example();

    return 0;
}

static void __exit module_unload(void)
{
    kern_log("Unloading C++ kernel module\n");

    release_cpp_subsystem_example();
}

void main() {}

module_init(module_load);
module_exit(module_unload);

MODULE_DESCRIPTION ("TFS Linux kernel module");
MODULE_VERSION ("0.1");
MODULE_AUTHOR ("Ronald Veldema");
MODULE_LICENSE ("GPL");