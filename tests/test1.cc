#include <gtest/gtest.h>

#include "tfs.hh"


using namespace Kernel_Block_API;
using namespace TFS_Kernel;

TEST(FS, get_root)
{
    auto bs = alloc_array<Kernel_Block_API::BlockStorage>(1);
    FileSystem fs(bs);

    Dir* d = fs.get_root();

    auto entries = d->get_dir_entries();

}
