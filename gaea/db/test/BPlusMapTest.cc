//
// Created by xi on 18-12-21.
//

#include <gaea/db/BPlusMap.h>
#include <iostream>

using namespace gaea;

int main()
{
    BPlusMap<int, int> map;
    map.InsertOrUpdate(1, 1);
    map.InsertOrUpdate(2, 2);
    auto ret = map.Find(2);
    auto ret2 = map.Find(3);
    std::cout << *ret << std::endl;
    std::cout << ret2.has_value();
    return 0;
}