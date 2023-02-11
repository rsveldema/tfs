#include <gtest/gtest.h>

#include "tfs.hh"


using namespace Kernel_Block_API;
using namespace TFS_Kernel;

TEST(FS, get_root)
{
    BlockStorage bs;
    FileSystem fs(bs);

    Dir& d = fs.get_root();
}