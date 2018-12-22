//
// Created by xi on 18-12-16.
//

#ifndef GAEA_BTREEN_H
#define GAEA_BTREEN_H

#include <string>
#include <sstream>
#include <gaea/tree/BPlusException.h>
#include <gaea/utils/noncopyable.h>
#include <gaea/tree/BPlusNode.h>

namespace gaea
{
namespace bplus
{

//order是B+树的阶最小值为3
//内部节点： children数量 = key_num + 1
//叶子节点： children数量 = key_num
//内部节点最多children数量比key数量多１满足 k[i+1] > p[i+1] >= k[i]
//叶子节点最多order - 1个子树，与key的数量相同，第order个指针指向同一层右边兄弟，最右指向nullptr
//InternalNode::mappings_[0] is a dummy header
//NOTE: NOT thread safe
template <typename K, typename V>
class BPlusTree
{
public:
    using EntryType = std::tuple<K, V, LeafNode<K, V>*>;

    explicit BPlusTree(int order = 4);

    ~BPlusTree();

    bool IsEmpty() const;

    Record<V>* Find(const K& key);

    //may be update
    void Insert(const K& key, const V& value);

//    void Remove(const K& key);

    //after this function has been called, you could still call InsertOrUpdate to build a new tree
    void DestroyTree();

    DISALLOW_COPY_AND_ASSIGN(BPlusTree);

private:
    void NewTree(const K& key, const V& value);

    LeafNode<K, V>* FindLeaf(const K& k);

    void InsertIntoLeaf(const K& key, const V& value);

    void InsertIntoParent(Node<K>* old_node, const K& key, Node<K>* new_node);

    template <typename T>
    T* Split(T* node);

private:
    const int order_;
    Node<K>* root_;
};

template<typename K, typename V>
BPlusTree<K, V>::BPlusTree(int order) :
    order_(order),
    root_(nullptr)
{}

template <typename K, typename V>
BPlusTree<K, V>::~BPlusTree()
{
    DestroyTree();
}

template <typename K, typename V>
Record<V>* BPlusTree<K, V>::Find(const K& key)
{
    if (IsEmpty())
    {
        return nullptr;
    }
    LeafNode<K, V>* leaf = FindLeaf(key);
    if (!leaf)
    {
        return nullptr;
    }
    return leaf->Lookup(key);
}

template <typename K, typename V>
bool BPlusTree<K, V>::IsEmpty() const
{
    return !root_;
}

template <typename K, typename V>
void BPlusTree<K, V>::Insert(const K& key, const V& value)
{
    if (IsEmpty())
    {
        NewTree(key, value);
    }
    else
    {
        InsertIntoLeaf(key, value);
    }
}

//TODO
//template <typename K, typename V>
//void BPlusTree<K, V>::Remove(const K& key)
//{
//
//}

template <typename K, typename V>
void BPlusTree<K, V>::DestroyTree()
{
    //NOTE: delete nullptr is legal
    delete root_;
    root_ = nullptr;
}

template<typename K, typename V>
void BPlusTree<K, V>::NewTree(const K& key, const V& value)
{
    LeafNode<K, V>* leaf = new LeafNode<K, V>(order_);
    leaf->CreateAndInsertRecord(key, value);
    root_ = leaf;
}

template<typename K, typename V>
LeafNode<K, V>* BPlusTree<K, V>::FindLeaf(const K& key)
{
    if (IsEmpty())
    {
        return nullptr;
    }
    Node<K>* node = root_;
    while (!node->IsLeaf())
    {
        InternalNode<K>* internal = static_cast<InternalNode<K>*>(node);
        node = internal->Lookup(key);
    }
    return static_cast<LeafNode<K, V>*>(node);
}

template<typename K, typename V>
void BPlusTree<K, V>::InsertIntoLeaf(const K& key, const V& value)
{
    LeafNode<K, V>* leaf = FindLeaf(key);
    if (!leaf)
    {
        throw LeafNotFoundException<K>(key);
    }
    Record<V>* record = leaf->Lookup(key);
    if (record)
    {
        record->SetValue(value);
        return;
    }
    int new_size = leaf->CreateAndInsertRecord(key, value);
    if (new_size > leaf->MaxSize())
    {
        LeafNode<K, V>* new_leaf = Split(leaf);
        new_leaf->SetNext(leaf->Next());
        leaf->SetNext(new_leaf);
        K new_key = new_leaf->FirstKey();
        InsertIntoParent(leaf, new_key, new_leaf);
    }
}


template<typename K, typename V>
void BPlusTree<K, V>::InsertIntoParent(Node<K>* old_node, const K& key, Node<K>* new_node)
{
    InternalNode<K>* parent = static_cast<InternalNode<K>*>(old_node->Parent());
    if (!parent)
    {
        root_ = new InternalNode<K>(order_);
        parent = static_cast<InternalNode<K>*>(root_);
        old_node->SetParent(parent);
        new_node->SetParent(parent);
        parent->PopulateNewRoot(old_node, key, new_node);
    }
    else
    {
        int new_size = parent->InsertNodeAfter(old_node, key, new_node);
        if (new_size > parent->MaxSize())
        {
            InternalNode<K>* new_parent_split = Split(parent);
            K new_key = new_parent_split->ReplaceAndReturnFirstKey();
            InsertIntoParent(parent, new_key, new_parent_split);
        }
    }
}

template<typename K, typename V>
template<typename T>
T* BPlusTree<K, V>::Split(T* node)
{
    T* new_node = new T(order_, node->Parent());
    node->MoveHalfTo(new_node);
    return new_node;
}

} //namespace bplus
} //namespace gaea

#endif //GAEA_BTREE_H
