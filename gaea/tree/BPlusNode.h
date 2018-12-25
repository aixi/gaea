//
// Created by xi on 18-12-21.
//

#ifndef GAEA_BPLUSNODE_H
#define GAEA_BPLUSNODE_H

#include <assert.h>
#include <vector>
#include <algorithm>

#include <gaea/tree/BPlusRecord.h>

namespace gaea
{
namespace bplus
{

template <typename K>
class Node
{
public:

    explicit Node(int order);
    Node(int order, Node* parent);

    virtual ~Node() = default;

    int Order() const;

    Node* Parent() const;

    void SetParent(Node* parent);

    bool IsRoot() const;

    virtual bool IsLeaf() const = 0;

    virtual int Size() const = 0;

    virtual int MinSize() const = 0;

    virtual int MaxSize() const = 0;

    virtual std::string DebugString(bool verbose = false) const = 0;

    virtual const K FirstKey() const = 0;

private:
    const int order_;
    Node* parent_;
};

template<typename K>
Node<K>::Node(int order) :
    order_(order)
{
    assert(order_ >= 3);
}

template<typename K>
Node<K>::Node(int order, Node *parent) :
    order_(order),
    parent_(parent)
{}

template<typename K>
int Node<K>::Order() const
{
    return order_;
}

template<typename K>
Node<K>* Node<K>::Parent() const
{
    return parent_;
}

template<typename K>
void Node<K>::SetParent(Node *parent)
{
    parent_ = parent;
}

template<typename K>
bool Node<K>::IsRoot() const
{
    return !parent_;
}

template <typename K, typename V>
class LeafNode : public Node<K>
{
public:
    using MappingType = std::pair<K, Record<V>*>;
    using EntryType = std::tuple<K, V, LeafNode*>;

    //FIXME: effective C++ 3e item 43
    //avoid to many this->Order()
    using Node<K>::Order;
    using Node<K>::Parent;

    explicit LeafNode(int order);

    LeafNode(int order, Node<K>* parent);

    ~LeafNode() override;

    bool IsLeaf() const override;

    LeafNode* Next() const;

    void SetNext(LeafNode* next);

    int Size() const override;

    int MinSize() const override;

    int MaxSize() const override;

    const K FirstKey() const override;

    void MoveHalfTo(LeafNode<K, V>* recipient);

    void CopyHalfFrom(std::vector<MappingType>& from);

    std::string DebugString(bool verbose) const override;

    void Insert(const K& key, Record<V>* record);

    Record<V>* Lookup(const K& key) const;

    int CreateAndInsertRecord(const K& key, const V& value);

    DISALLOW_COPY_AND_ASSIGN(LeafNode);

private:
    LeafNode* next_;
    std::vector<MappingType> mappings_;
};

template<typename K, typename V>
LeafNode<K, V>::LeafNode(int order) :
    Node<K>(order),
    next_(nullptr)
{

}

template<typename K, typename V>
LeafNode<K, V>::LeafNode(int order, Node<K> *parent) :
    Node<K>(order, parent),
    next_(nullptr)
{}

template<typename K, typename V>
LeafNode<K, V>::~LeafNode()
{
    for (auto& mapping : mappings_)
    {
        delete mapping.second;
    }
}

template<typename K, typename V>
bool LeafNode<K, V>::IsLeaf() const
{
    return true;
}

template<typename K, typename V>
LeafNode<K, V>* LeafNode<K, V>::Next() const
{
    return next_;
}

template<typename K, typename V>
void LeafNode<K, V>::SetNext(LeafNode* next)
{
    next_ = next;
}

template<typename K, typename V>
int LeafNode<K, V>::Size() const
{
    return static_cast<int>(mappings_.size());
}

template<typename K, typename V>
int LeafNode<K, V>::MinSize() const
{
    return Order() / 2;
}

template<typename K, typename V>
int LeafNode<K, V>::MaxSize() const
{
    return Order() - 1;
}

template<typename K, typename V>
const K LeafNode<K, V>::FirstKey() const
{
    assert(!mappings_.empty());
    return mappings_.front().first;
}

template<typename K, typename V>
std::string LeafNode<K, V>::DebugString(bool verbose) const
{
    std::ostringstream oss;
    if (verbose)
    {
        oss << "[" << std::hex << this << std::dec << "]<" << mappings_.size() << "> ";
    }
    bool is_first = true;
    for (const auto& mapping : mappings_)
    {
        if (is_first)
        {
            is_first = false;
        }
        else
        {
            oss << " ";
        }
        oss << mapping.first;
    }
    if (verbose)
    {
        oss << "[" << std::hex << next_ << ">";
    }
    return oss.str();
}

template <typename K, typename V>
void LeafNode<K, V>::MoveHalfTo(LeafNode<K, V>* recipient)
{
    recipient->CopyHalfFrom(mappings_);
    mappings_.erase(mappings_.begin() + MinSize(), mappings_.end());
}

template <typename K, typename V>
void LeafNode<K, V>::CopyHalfFrom(std::vector<typename LeafNode<K, V>::MappingType>& from)
{
    mappings_.insert(mappings_.end(), from.begin() + MinSize(), from.end());
}

template<typename K, typename V>
void LeafNode<K, V>::Insert(const K& key, Record<V>* record)
{
    auto pos = mappings_.begin();
    while (pos != mappings_.end() && key > pos->first)
    {
        ++pos;
    }
    mappings_.insert(pos, LeafNode<K, V>::MappingType(key, record));
}

template<typename K, typename V>
Record<V>* LeafNode<K, V>::Lookup(const K& key) const
{
    for (const auto& mapping : mappings_)
    {
        if (mapping.first == key)
        {
            return mapping.second;
        }
    }
    return nullptr;
}

template<typename K, typename V>
int LeafNode<K, V>::CreateAndInsertRecord(const K& key, const V& value)
{
    Record<V>* record = Lookup(key);
    if (!record)
    {
        Record<V>* new_record = new Record<V>(value);
        Insert(key, new_record);
    }
    return static_cast<int>(mappings_.size());
}

template <typename K>
class InternalNode : public Node<K>
{
public:
    using MappingType = std::pair<K, Node<K>*>;

    using Node<K>::Order;
    using Node<K>::Parent;

    explicit InternalNode(int order);

    explicit InternalNode(int order, Node<K>* parent);

    ~InternalNode() override;

    bool IsLeaf() const override;

    int Size() const override;

    int MinSize() const override;

    int MaxSize() const override;

    void MoveHalfTo(InternalNode* recipient);

    K ReplaceAndReturnFirstKey();

    const K FirstKey() const override;

    void PopulateNewRoot(Node<K>* old_node, const K& new_key, Node<K>* new_node);

    int InsertNodeAfter(Node<K>* pos_node, const K& new_key, Node<K>* new_node);

    std::string DebugString(bool verbose) const override;

    Node<K>* Lookup(const K& key) const;

    DISALLOW_COPY_AND_ASSIGN(InternalNode);

private:
    void CopyHalfFrom(const std::vector<MappingType>& from);

private:
    std::vector<MappingType> mappings_;
};

template <typename K>
InternalNode<K>::InternalNode(int order) :
    Node<K>(order)
{}

template <typename K>
InternalNode<K>::InternalNode(int order, Node<K>* parent) :
    Node<K>(order, parent)
{}

template <typename K>
InternalNode<K>::~InternalNode()
{
    for (auto& mapping : mappings_)
    {
        delete mapping.second;
    }
}

template<typename K>
bool InternalNode<K>::IsLeaf() const
{
    return false;
}

template<typename K>
int InternalNode<K>::Size() const
{
    return static_cast<int>(mappings_.size());
}

template<typename K>
int InternalNode<K>::MinSize() const
{
    return (Order() + 1) / 2;
}

template<typename K>
int InternalNode<K>::MaxSize() const
{
    //includes the first entry, which has a dummy key
    //a node whose keys are all < k1
    return Order();
}

template <typename K>
void InternalNode<K>::PopulateNewRoot(Node<K>* old_node, const K& new_key, Node<K>* new_node)
{
    static_assert(std::is_default_constructible<K>::value, "Key type is not default constructible");
    mappings_.push_back(std::make_pair(K(), old_node));
    mappings_.push_back(std::make_pair(new_key, new_node));
}

template <typename K>
int InternalNode<K>::InsertNodeAfter(Node<K>* pos_node, const K& new_key, Node<K>* new_node)
{
    auto pos = mappings_.begin();
    for (; pos != mappings_.end() && pos->second != pos_node; ++pos);
    mappings_.insert(pos + 1, std::make_pair(new_key, new_node));
    return static_cast<int>(mappings_.size());
}

template <typename K>
K InternalNode<K>::ReplaceAndReturnFirstKey()
{
    assert(!mappings_.empty());
    K new_key = mappings_.front().first;
    static_assert(std::is_default_constructible<K>::value, "Key type is not default constructible");
    mappings_.front().first = K();
    return new_key;
}

template <typename K>
void InternalNode<K>::MoveHalfTo(InternalNode<K>* recipient)
{
    recipient->CopyHalfFrom(mappings_);
    mappings_.erase(mappings_.begin() + MinSize(), mappings_.end());
}

template <typename K>
void InternalNode<K>::CopyHalfFrom(const std::vector<typename InternalNode<K>::MappingType>& from)
{
    for (size_t i = MinSize(); i < from.size(); ++i)
    {
        from[i].second->SetParent(this);
        mappings_.push_back(from[i]);
    }
}

template <typename K>
const K InternalNode<K>::FirstKey() const
{
    assert(mappings_.size() >= 2);
    //the mappings_[0] is a dummy node
    return mappings_[1].first;
}

template <typename K>
Node<K>* InternalNode<K>::Lookup(const K& key) const
{
    auto pos = mappings_.begin() + 1;
    //first element in mappings_ is dummy
    while (pos != mappings_.end() && key >= pos->first)
    {
        ++pos;
    }
    //if reach mappings_.end() it still work
    --pos;
    return pos->second;
}

template <typename K>
std::string InternalNode<K>::DebugString(bool verbose) const
{
    if (mappings_.empty())
    {
        return std::string(""); //RVO
    }
    std::ostringstream oss;
    if (verbose)
    {
        oss << "[" << std::hex << this << std::dec << "]<" << mappings_.size() << "> ";
    }
    auto begin = verbose ? mappings_.begin() : mappings_.begin() + 1;
    auto end = mappings_.end();
    bool is_first = true;
    while (begin != end)
    {
        if (is_first)
        {
            is_first = false;
        }
        else
        {
            oss << " ";
        }
        oss << std::dec << begin->first;
        if (verbose)
        {
            oss << "(" << std::hex << begin->second << std::dec << ")";
        }
        ++begin;
    }
    return oss.str();
}


} //namespace bplus
} //namespace gaea

#endif //GAEA_BPLUSNODE_H
