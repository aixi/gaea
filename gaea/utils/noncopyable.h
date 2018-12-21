//
// Created by xi on 18-12-20.
//

#ifndef GAEA_UTIL_H
#define GAEA_UTIL_H

#ifndef DISALLOW_COPY_AND_ASSIGN
    #define DISALLOW_COPY_AND_ASSIGN(T) \
       T(T const&) = delete; \
       T(T&&) = delete; \
       T& operator=(T const&) = delete; \
       T& operator=(T&&) = delete
#endif

#endif //GAEA_UTIL_H
