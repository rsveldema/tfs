#pragma once

typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long u64;

extern "C" {
    void tfs_panic(const char* msg);
    void *tfs_alloc(u32 size);
    void tfs_free(void* ptr);
}

#define tfs_assert(x)  if(!(x)) tfs_panic(#x)

#if KERNEL_MODE
void *operator new(u64 size, void*& ptr);
#else
#include <new>
#endif


#define TFS_BLOCK_SIZE (16 * 1024)

namespace Kernel_Block_API
{
    struct Bio
    {
    };

    struct BlockStorage
    {
        void submit_bio(Bio *bio);
    };
}

namespace TFS_Kernel
{

    template <typename T>
    struct Array
    {
        u32 length;
        T elts[0];


        const T& get(u32 ix) const
        {
            tfs_assert(ix < length);
            return elts[ix];
        }

        T& get(u32 ix)
        {
            tfs_assert(ix < length);
            return elts[ix];
        }

        void set(u32 ix, const T& elt)
        {
            elts[ix] = elt;
        }
    };

    using String = Array<char>;

    template<typename T>
    Array<T>* alloc_array(u32 num_elts)
    {
        auto* ret = (Array<T>*) tfs_alloc(sizeof(Array<T>) + (num_elts * sizeof(T)));
        ret->length = num_elts;
        return ret;
    }

    template<typename T, typename A1>
    T* alloc(const A1& arg1)
    {
        void *storage = tfs_alloc(sizeof(T));
        if (! storage) {
            return nullptr;
        }
        return new(storage) T(arg1);
    }

    struct DirEntry
    {
        String m_name;
        u32 m_permissions; // r/w/x
        u32 m_owner_id;
        u32 m_group_id;
    };

    struct File
    {
        u32 m_inode_id;

        File(u32 _inode_id) : m_inode_id(_inode_id)
        {}

        void read(u32 offset, u32 num_bytes);
        void write(u32 offset, u32 num_bytes, void *data);
    };

    struct Dir
    {
        File* m_file;
        Array<DirEntry> *m_data = nullptr;

        Array<DirEntry> *get_dir_entries();

        Dir(File* file);
        ~Dir();
    };

    struct Superblock
    {
        u32 m_magic = 0xabcdef1e;
    };

    /** Disk 1:
     *     Superblock
     *     Dir root
     *
     * Disk 2:
     *     Superblock
     *
     */
    struct FileSystem
    {
        Array<Kernel_Block_API::BlockStorage> *m_storage_array;
        Dir* m_root = nullptr;

        FileSystem(Array<Kernel_Block_API::BlockStorage> *storage_array) : m_storage_array(storage_array) {}

        Dir *get_root();
    };
}