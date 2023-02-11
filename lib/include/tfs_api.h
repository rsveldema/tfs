

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
        ino_t d_ino;             /* Inode number */
        off_t d_off;             /* Not an offset; see below */
        unsigned short d_reclen; /* Length of this record */
        unsigned char d_type;    /* Type of file; not supported
                                     by all filesystem types */
        char d_name[256];        /* Null-terminated filename */
    };

    DIR *opendir(const char *name);
    dirent *readdir(DIR *dirp);
    int closedir(DIR *dir);

    int open(const char *pathname, int flags, mode_t mode);
    ssize_t read(int fd, void *buf, size_t count);
    ssize_t write(int fd, const void *buf, size_t count);
    int close(int fd);
}