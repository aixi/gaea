//
// Created by xi on 18-12-22.
//

#include <gaea/utils/FileUtil.h>

using namespace gaea;

int main()
{
    file_util::ReadSmallFile file("/home/xi/img.jpg");
    std::string content;
    int64_t a, b, c;
    file.ReadToString(100, &content, &a, &b, &c);
    return 0;
}