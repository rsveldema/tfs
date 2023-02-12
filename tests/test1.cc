#include <gtest/gtest.h>

#include "tfs.hh"

using namespace Kernel_Block_API;
using namespace TFS_Kernel;


TEST(FS, get_root)
{
    auto bs = alloc_array<BlockStorage>(static_cast<u32>(BlockStorageIndex::MIN_NEEDED_BLOCK_STORAGE));
    FileSystem fs(bs);

    Dir *d = fs.get_root();

    auto entries = d->get_dir_entries();
}
