//
// Created by xi on 18-12-17.
//

#include <iostream>
#include <gaea/tree/AVLTree.h>

using namespace gaea;

//check Insert and Rotate
//https://www.cnblogs.com/skywang12345/p/3577360.html

int main()
{
    AVLTree<int> tree;
    tree.Insert(3);
    tree.Insert(2);
    tree.Insert(1); //RRRotate
    tree.Insert(4);
    tree.Insert(5); //LLRotate
    tree.Insert(6); //LLRotate
    tree.Insert(7); //LLRotate
    tree.Insert(16);
    tree.Insert(15); //LLRotate
    tree.Insert(14);//RLRotate
    return 0;
}