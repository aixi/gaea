//
// Created by xi on 18-12-22.
//

#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gaea/utils/FileUtil.h>
#include <gaea/utils/types.h>

namespace gaea
{
namespace file_util
{


ReadSmallFile::ReadSmallFile(const std::string& filename) :
    fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
    err_(0)
{
    buf_[0] = '\0';
    if (fd_ < 0)
    {
        err_ = errno;
    }
}

ReadSmallFile::~ReadSmallFile()
{
    if (fd_ >= 0)
    {
        ::close(fd_); //FIXME: check EINTR
    }
}

template <typename String>
int ReadSmallFile::ReadToString(int max_size, String* content,
                                              int64_t* file_size,
                                              int64_t* modify_time,
                                              int64_t* create_time)
{
    static_assert(sizeof(off_t) == 8, "sizeof(off_t) != 8");
    assert(content != nullptr);
    int err = err_;
    if (fd_ >= 0)
    {
        if (file_size)
        {
            struct stat statbuf;
            if (::fstat(fd_, &statbuf) == 0)
            {
                if (S_ISREG(statbuf.st_mode))
                {
                    *file_size = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min(implicit_cast<int64_t>(max_size), *file_size)));
                }
                else if (S_ISDIR(statbuf.st_mode))
                {
                    err = EISDIR;
                }
                if (modify_time)
                {
                    *modify_time = statbuf.st_mtime;
                }
                if (create_time)
                {
                    *create_time = statbuf.st_ctime;
                }
            }
            else
            {
                err = errno;
            }
        }
        while (content->size() < implicit_cast<size_t>(max_size))
        {
            size_t to_read = std::min(implicit_cast<size_t>(max_size) - content->size(), sizeof(buf_));
            ssize_t n = ::read(fd_, buf_, to_read);
            if (n > 0)
            {
                content->append(buf_, n);
            }
            else
            {
                if (n < 0)
                {
                    err = errno;
                }
                break;
            }
        }
    }
    return err;
}

int ReadSmallFile::ReadToBuffer(int* size)
{
    int err = err_;
    if (fd_ >= 0)
    {
        ssize_t n = ::pread(fd_, buf_, sizeof(buf_) - 1, 0);
        if (n >= 0)
        {
            if (size)
            {
                *size = static_cast<int>(n);
            }
            buf_[n] = '\0';
        }
        else
        {
            err = errno;
        }
    }
    return err;
}

//explicit instantiation
template int ReadSmallFile::ReadToString(int max_size, std::string* content, int64_t *file_size, int64_t *modify_time, int64_t *create_time);

} //namespace file_util
} //namespace gaea