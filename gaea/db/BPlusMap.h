//
// Created by xi on 18-12-21.
//

#ifndef GAEA_MEMTABLE_H
#define GAEA_MEMTABLE_H

#include <optional>
#include <gaea/tree/BPlusTree.h>

namespace gaea
{

template <typename K, typename V>
class BPlusMap
{
public:
    explicit BPlusMap(int order = 4);

    void InsertOrUpdate(const std::pair<K, V>& pair);

    void InsertOrUpdate(const K& key, const V& value);

    std::optional<V> Find(const K& key);

    DISALLOW_COPY_AND_ASSIGN(BPlusMap);

private:
    bplus::BPlusTree<K, V> tree_;
};

template <typename K, typename V>
BPlusMap<K, V>::BPlusMap(int order) : tree_(order)
{}

template <typename K, typename V>
void BPlusMap<K, V>::InsertOrUpdate(const std::pair<K, V>& pair)
{
    InsertOrUpdate(pair.first, pair.second);
}

template <typename K, typename V>
void BPlusMap<K, V>::InsertOrUpdate(const K& key, const V& value)
{
    tree_.Insert(key, value);
}

template<typename K, typename V>
std::optional<V> BPlusMap<K, V>::Find(const K& key)
{
    bplus::Record<V>* ret = tree_.Find(key);
    return ret == nullptr ? std::nullopt : std::optional<V>(ret->Value());
}

} //namespace gaea

#endif //GAEA_MEMTABLE_H
