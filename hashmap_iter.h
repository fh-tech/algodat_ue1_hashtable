//
// Created by daniel on 15.03.18.
//

#ifndef DEFINITIVEHASH_HASHMAP_ITER_H
#define DEFINITIVEHASH_HASHMAP_ITER_H

#include "hashmap.h"

template <typename T, typename C>
class internal_iter_impl {
    size_t m_i;
    hash_t m_hash;
    C& m_data;

public:
    internal_iter_impl(hash_t hash, C& v, size_t step)
        : m_hash(hash)
        , m_data(v)
        , m_i(step)
    {
    }

    bool operator!=(internal_iter_impl& other)
    {
        return other.m_i != m_i;
    }

    T& operator*() const
    {
        return m_data[(m_hash + m_i * m_i) % m_data.capacity()];
    }

    internal_iter_impl const& operator++()
    {
        ++m_i;
        return *this;
    }
};

using iterator = internal_iter_impl<Bucket, std::vector<Bucket>>;
using const_iterator = internal_iter_impl<Bucket, std::vector<Bucket> const>;

class hashiter {

    hash_t m_hash;
    std::vector<Bucket>& m_data;

public:
    hashiter(hash_t hash, std::vector<Bucket>& data)
        : m_data(data)
        , m_hash(hash)
    {
    }

    inline iterator begin()
    {
        return internal_iter_impl<Bucket, std::vector<Bucket>>(m_hash, m_data, 0);
    }

    inline iterator end()
    {
        return internal_iter_impl<Bucket, std::vector<Bucket>>(m_hash, m_data, m_data.capacity());
    }
};

#endif //DEFINITIVEHASH_HASHMAP_ITER_H
