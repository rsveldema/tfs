#include <stdlib.h>
#include <stdio.h>

#include "tfs.hh"

using namespace Kernel_Block_API;
using namespace TFS_Kernel;

void tfs_panic(const char *msg)
{
    fprintf(stderr, "PANIC: %s\n", msg);
    abort();
}

void *tfs_alloc(u32 size) { return malloc(size); }

void tfs_free(void *ptr) { free(ptr); }