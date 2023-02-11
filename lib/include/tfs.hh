#pragma once

typedef unsigned short int u16;
typedef unsigned int u32;

#define TFS_BLOCK_SIZE (16 * 1024)

namespace Kernel_Block_API
{
    struct Bio
    {
    };

    struct BlockStorage
    {
        void submit_bio(Bio* bio);
    };
}

namespace TFS_Kernel
{
    struct String
    {
        u16 length;
        char data[1];
    };

    struct DirEntry
    {
        String name;
        u32 permissions; // r/w/x
        u32 owner_id;
        u32 group_id;
    };

    struct File
    {
        void read(u32 offset, u32 num_bytes);
        void write(u32 offset, u32 num_bytes, void *data);
    };

    struct Dir
    {
        File file;
        u32 num_entries;

        DirEntry get_entry();
    };

    struct Superblock
    {
        u32 magic = 0xabcdef1e;
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
        Kernel_Block_API::BlockStorage& block_storage;

        Dir root;

        FileSystem(Kernel_Block_API::BlockStorage& block_storage) : block_storage(block_storage) {}

        Dir& get_root() { return root; }
    };
}

namespace TFS_User
{
    typedef int mode_t;
    typedef long long ssize_t;
    typedef unsigned long long size_t;

    typedef size_t ino_t;
    typedef size_t off_t;

    struct DIR
    {
        int fd;
    };

    struct dirent
    {
        ino_t          d_ino;       /* Inode number */
        off_t          d_off;       /* Not an offset; see below */
        unsigned short d_reclen;    /* Length of this record */
        unsigned char  d_type;      /* Type of file; not supported
                                        by all filesystem types */
        char           d_name[256]; /* Null-terminated filename */
    };


    DIR* opendir(const char* name);
    dirent *readdir(DIR *dirp);
    int closedir(DIR* dir);

    int open(const char *pathname, int flags, mode_t mode);
    ssize_t read(int fd, void *buf, size_t count);
    ssize_t write(int fd, const void *buf, size_t count);
    int close(int fd);
}