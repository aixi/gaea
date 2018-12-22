//
// Created by xi on 18-12-17.
//
#include <iostream>
#include <gaea/tree/BPlusTree.h>

using gaea::bplus::BPlusTree;

int main()
{
    BPlusTree<int, int> tree;
    for (int i = -999999; i < 1000000; ++i)
    {
        tree.Insert(i, i);
        int ret = tree.Find(i)->Value();
        assert(i == ret);
        tree.Insert(i, i + 1);
        int ret2 = tree.Find(i)->Value();
        assert(i + 1 == ret2);
    }
}


