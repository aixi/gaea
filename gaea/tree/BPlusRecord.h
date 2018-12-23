//
// Created by xi on 18-12-21.
//

#ifndef GAEA_BPLUSRECORD_H
#define GAEA_BPLUSRECORD_H

namespace gaea
{
namespace bplus
{

template <typename V>
class Record
{
public:
    explicit Record(const V& v);

    const V& Value() const;

    void SetValue(const V& v);

    void SetDeletion(bool flag);

    std::string DebugString() const;

    //default copy and assign are OK

private:
    V value_;
    bool deleted_;
};

template <typename V>
Record<V>::Record(const V& v) :
    value_(v),
    deleted_(false)
{}

template <typename V>
const V& Record<V>::Value() const
{
    return value_;
}

template <typename V>
void Record<V>::SetValue(const V& v)
{
    value_ = v;
}

template <typename V>
void Record<V>::SetDeletion(bool flag)
{
    deleted_ = flag;
}

template <typename V>
std::string Record<V>::DebugString() const
{
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}


} //namespace bplus
} //namespace gaea

#endif //GAEA_BPLUSRECORD_H
