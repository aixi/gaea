//
// Created by xi on 18-12-22.
//

// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

//Taken from muduo

#ifndef GAEA_FILES_H
#define GAEA_FILES_H

#include <sys/types.h> //off_t
#include <string>
#include <gaea/utils/noncopyable.h>

namespace gaea
{
namespace file_util
{

//file < 64KB
class ReadSmallFile
{
public:
    ReadSmallFile(const std::string& filename);

    ~ReadSmallFile();

    //return errno
    template <typename String>
    int ReadToString(int max_size, String* content,
                                   int64_t* file_size,
                                   int64_t* modify_time,
                                   int64_t* create_time);

    //Read at maximum kBufferSie into buf_
    //return errno
    int ReadToBuffer(int *size);

    const char *Buffer() const
    {
        return buf_;
    }

    static const int kBufferSize = 64 * 1024;

    DISALLOW_COPY_AND_ASSIGN(ReadSmallFile);

private:
    const int fd_;
    int err_;
    char buf_[kBufferSize];
};

} //namespace file_util
} //namespace gaea

#endif //GAEA_FILES_H
