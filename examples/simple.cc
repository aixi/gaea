#include <gaea/tree/BPlusTree.h>
#include <iostream>

using namespace gaea::bplus;

int main()
{
    BPlusTree<int, int> tree;
    for (int i = -9999999; i < 9999999; ++i)
    {
        tree.Insert(i, i);
        int ret = tree.Find(i)->Value();
        std::cout << (i == ret) << std::endl;
    }
    return 0;
}