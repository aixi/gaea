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

    std::string DebugString() const;

    //default copy and assign are OK

private:
    V value;
};

template <typename V>
Record<V>::Record(const V& v) : value(v)
{}

template <typename V>
const V& Record<V>::Value() const
{
    return value;
}

template <typename V>
void Record<V>::SetValue(const V& v)
{
    value = v;
}

template <typename V>
std::string Record<V>::DebugString() const
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


} //namespace bplus
} //namespace gaea

#endif //GAEA_BPLUSRECORD_H
