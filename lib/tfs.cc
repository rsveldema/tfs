#include <tfs.hh>


#if KERNEL_MODE
void *operator new(u64 size, void*& ptr)
{
    return ptr;
}
#endif

 namespace TFS_Kernel
{
    Array<DirEntry>* Dir::get_dir_entries()
    {
        return m_data;
    }

    Dir::Dir(File* file) : m_file(file)
    {
        m_data = alloc_array<DirEntry>(0);
    }

    Dir::~Dir()
    {
        tfs_free(m_data);
    }

    INode BlockStorage::get_as_inode(u32 inode_id)
    {
        INode ret {};
        return ret;
    }


     void File::read(Array<BlockStorage> *storage_array, u32 offset, u32 num_bytes)
     {

     }

     void File::write(Array<BlockStorage> *storage_array, u32 offset, u32 num_bytes, void *data)
     {

     }

    Dir *FileSystem::get_root()
    {
        if (! m_root)
        {
            u32 inode_id = 0;

            auto& inode_storage = m_storage_array->get(0);

            auto inode = inode_storage.get_as_inode(inode_id);

            auto file_size = inode.get_file_size();

            auto *file = alloc<File>(inode_id);

            file->read(m_storage_array, 0, file_size);

            m_root = alloc<Dir>(file);
        }
        return m_root;
    }
}