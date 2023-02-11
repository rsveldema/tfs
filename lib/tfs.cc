#include <tfs.hh>
#include <stdlib.h>



namespace TFS_Kernel
{
    void *tfs_alloc(u32 size)
    {
        return malloc(size);
    }

    void tfs_free(void* ptr)
    {
        free(ptr);
    }

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

    Dir *FileSystem::get_root()
    {
        if (! m_root)
        {
            File *file = alloc<File>(0);

            auto& root_storage = m_storage_array->get(0);

            m_root = alloc<Dir>(file);
        }
        return m_root;
    }
}