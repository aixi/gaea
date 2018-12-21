//
// Created by xi on 18-12-17.
//

#ifndef GAEA_AVLTREE_H
#define GAEA_AVLTREE_H

#include <stddef.h>
#include <memory>

namespace gaea
{

template <typename T>
struct AVLTreeNode
{
    T key;
    AVLTreeNode* left;
    AVLTreeNode* right;
    bool deleted;
    int height;

    AVLTreeNode(const T& k, AVLTreeNode* l, AVLTreeNode* r) :
        key(k),
        left(l),
        right(r),
        deleted(false)
    {}

    ~AVLTreeNode()
    {
        std::cout << "~AVLTreeNode()" << std::endl;
    }
};

template <typename T>
class AVLTree
{
public:
    AVLTree();

    ~AVLTree();

    int Height();

    //void PreOrder() const;

    //void InOrder() const;

    //void PostOrder() const;

    AVLTreeNode<T>* Search(const T& key) const;

//    const T& Minimum() const;
//
//    const T& Maximum() const;

    void Insert(const T& key);

    void LazyRemove(const T& key);

    void Remove(const T& key);

    void Destroy();

//    void Print();

    //noncopyable
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

private:
    int Height(AVLTreeNode<T>* node) const;

    AVLTreeNode<T>* Search(AVLTreeNode<T>* node, const T& key) const;

    AVLTreeNode<T>* LLRotate(AVLTreeNode<T>* k2);

    AVLTreeNode<T>* RRRotate(AVLTreeNode<T>* k1);

    AVLTreeNode<T>* LRRotate(AVLTreeNode<T>* k3);

    AVLTreeNode<T>* RLRotate(AVLTreeNode<T>* k1);

    //duplicate Key is not allowed
    AVLTreeNode<T>* Insert(AVLTreeNode<T>*& tree_root, const T& key);

    //always lazy remove
    //AVLTreeNode<T>* Remove(AVLTreeNode<T>*& tree_root, AVLTreeNode<T>* target);

    void Destroy(AVLTreeNode<T>* tree_root);

private:
    AVLTreeNode<T>* root_;
};

template <typename T>
AVLTree<T>::AVLTree() :
    root_(nullptr)
{}

template <typename T>
AVLTree<T>::~AVLTree()
{
    Destroy();
}

template <typename T>
int AVLTree<T>::Height()
{
    return Height(root_);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::Search(const T& key) const
{
    return Search(root_, key);
}

template <typename T>
void AVLTree<T>::Insert(const T& key)
{
    Insert(root_, key);
}

template <typename T>
void AVLTree<T>::Remove(const T& key)
{
    LazyRemove(key);
}

template <typename T>
void AVLTree<T>::Destroy()
{
    Destroy(root_);
}

template <typename T>
int AVLTree<T>::Height(gaea::AVLTreeNode<T>* node) const
{
    if (node)
    {
        return node->height;
    }
    else
    {
        return 0;
    }
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::Search(AVLTreeNode<T>* node, const T& key) const
{

    while (node && node->key != key)
    {
        if (key < node->key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return node;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::LLRotate(AVLTreeNode<T>* k1)
{
    AVLTreeNode<T>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = std::max(Height(k1->left), Height(k1->left)) + 1;
    k2->height = std::max(Height(k2->right), k1->height) + 1;
    return k2;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::RRRotate(AVLTreeNode<T>* k2)
{
    AVLTreeNode<T>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = std::max(Height(k2->left), Height(k2->right)) + 1;
    k1->height = std::max(Height(k1->left), k2->height) + 1;
    return k1;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::LRRotate(AVLTreeNode<T>* k3)
{
    k3->left = LLRotate(k3->left);
    return RRRotate(k3);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::RLRotate(AVLTreeNode<T>* k1)
{
    k1->right = RRRotate(k1->right);
    return LLRotate(k1);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::Insert(AVLTreeNode<T>*& tree_root, const T& key)
{
    if (!tree_root)
    {
        tree_root = new AVLTreeNode<T>(key, nullptr, nullptr);
    }
    else if (key < tree_root->key)
    {
        tree_root->left = Insert(tree_root->left, key);
        if ((Height(tree_root->left) - Height(tree_root->right)) >= 2)
        {
            if (key < tree_root->left->key)
            {
                tree_root = RRRotate(tree_root);
            }
            else
            {
                tree_root = LRRotate(tree_root);
            }
        }
    }
    else if (key > tree_root->key)
    {
        tree_root->right = Insert(tree_root->right, key);
        if ((Height(tree_root->right) - Height(tree_root->left)) >= 2)
        {
            if (key > tree_root->right->key)
            {
                tree_root = LLRotate(tree_root);
            }
            else
            {
                tree_root = RLRotate(tree_root);
            }
        }
    }
    // duplicate key is not allowed
    tree_root->height = std::max(Height(tree_root->left), Height(tree_root->right)) + 1;
    return tree_root;
}

template <typename T>
void AVLTree<T>::Destroy(AVLTreeNode<T>* tree_root)
{

    if (tree_root)
    {
        Destroy(tree_root->left);
        Destroy(tree_root->right);
    }
    delete tree_root;
}

template <typename T>
void AVLTree<T>::LazyRemove(const T& key)
{
    AVLTreeNode<T>* node = Search(key);
    if (node)
    {
        node->deleted = true;
    }
}

//template <typename T>
//AVLTreeNode<T>* AVLTree<T>::Remove(AVLTreeNode<T>*& tree_root, AVLTreeNode<T>* target)
//{
//    if (!tree_root || !target)
//    {
//        return nullptr;
//    }
//    if (target->key > tree_root->key) //in right sub tree
//    {
//        tree_root->right = Remove(tree_root->right, target);
//        if ((Height(tree_root->left) - Height(tree_root->right)) >= 2)
//        {
//            AVLTreeNode<T>* lchild = tree_root->left;
//            if (Height(lchild->left) > Height(lchild->right))
//            {
//                tree_root = RRRotate(tree_root);
//            }
//            else
//            {
//                tree_root = LRRotate(tree_root);
//            }
//        }
//    }
//    else if (target->key < tree_root->key) //in left sub tree
//    {
//        tree_root->left = Remove(tree_root->left, target);
//        if ((Height(tree_root->right) - Height(tree_root->left) >= 2))
//        {
//            AVLTreeNode<T>* rchild = tree_root->right;
//            if (Height(rchild->right) > Height(rchild->left))
//            {
//                tree_root = LLRotate(tree_root);
//            }
//            else
//            {
//                tree_root = RLRotate(tree_root);
//            }
//        }
//    }
//    else //tree_root is the target
//    {
//        if (tree_root->left && tree_root->right) //has 2 children_
//        {
//
//        }
//        else if (!tree_root->left && !tree_root->right) //no child
//        {
//            delete tree_root;
//            tree_root = nullptr;
//        }
//        else //has exactly 1 child
//        {
//
//        }
//    }
//}

} //namespace gaea

#endif //GAEA_AVLTREE_H
