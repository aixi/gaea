//
// Created by xi on 18-12-17.
//
#include <iostream>
#include <gaea/tree/BPlusTree.h>

using namespace gaea::bplus;

int main()
{
//    LeafNode<int, int> a(3);
//    InternalNode<int> b(3);
//    BPlusTree<int, int> tree;
//    tree.InsertOrUpdate(-1, 1);
//    tree.InsertOrUpdate(-2, 1);
//    tree.InsertOrUpdate(-3, 1);
//    tree.InsertOrUpdate(4, 1);
//    tree.InsertOrUpdate(5, 1);
//    tree.InsertOrUpdate(6, 1);
//    tree.InsertOrUpdate(7, 1);
//    tree.InsertOrUpdate(8, 1);
//    tree.InsertOrUpdate(9, 1);
//    std::cout << 1;
    BPlusTree<std::string, std::string> tree;
    tree.Insert("1", "v");
    tree.Insert("2", "v");
    tree.Insert("3", "v");
    tree.Insert("4", "v");
    tree.Insert("5", "v");
    tree.Insert("6", "v");
    tree.Insert("7", "v");
    tree.Insert("8", "v");
    tree.Insert("9", "v");
    return 0;
}
