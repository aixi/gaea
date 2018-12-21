//
// Created by xi on 18-12-21.
//

#ifndef GAEA_BPLUSEXCEPTION_H
#define GAEA_BPLUSEXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

namespace gaea
{
namespace bplus
{

template <typename K>
class LeafNotFoundException : public std::exception
{
public:
    explicit LeafNotFoundException(const K& key);
    const char* what() const noexcept override;
private:
    K key_;
};

template <typename K>
class NodeNotFoundException : public std::exception
{
public:
    explicit NodeNotFoundException(const std::string& searched_node, const std::string& containing_node);
    const char* what() const noexcept override;

private:
    std::string searched_node_;
    std::string containing_node_;
};

template <typename K>
class RecordNotFoundException : public std::exception
{
public:
    explicit RecordNotFoundException(const K& key);
    const char* what() const noexcept override;

private:
    K key_;
};

template <typename K>
LeafNotFoundException<K>::LeafNotFoundException(const K& key) : key_(key)
{}

template <typename K>
const char* LeafNotFoundException<K>::what() const noexcept
{
    std::ostringstream oss;
    oss << "Key not found in any leaf node:   ";
    oss << key_;
    static std::string msg(oss.str());
    return msg.c_str();
}

template <typename K>
NodeNotFoundException<K>::NodeNotFoundException(const std::string& searched_node, const std::string& containing_node) :
    searched_node_(searched_node),
    containing_node_(containing_node)
{}

template <typename K>
const char* NodeNotFoundException<K>::what() const noexcept
{
    std::ostringstream oss;
    oss << "Node |" << searched_node_ << "| not found";
    oss << " as a child of node ";
    oss << containing_node_;
    static std::string msg(oss.str());
    return msg.c_str();
}

template <typename K>
RecordNotFoundException<K>::RecordNotFoundException(const K& key) : key_(key)
{}

template <typename K>
const char* RecordNotFoundException<K>::what() const noexcept
{
    std::ostringstream oss;
    oss << "Record not found with key: " << key_;
    static std::string msg(oss.str());
    return msg.c_str();
}

}
}

#endif //GAEA_BPLUSEXCEPTION_H
