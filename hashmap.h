//
// Created by daniel on 15.03.18.
//
#ifndef DEFINITIVEHASH_HASHMAP_H
#define DEFINITIVEHASH_HASHMAP_H

#include "Share.h"
#include <bits/unique_ptr.h>
#include <vector>

using hash_t = uint32_t;

struct Bucket {
    Share* data = nullptr;
    Bucket* other = nullptr;
};

struct QuadraticProbingIterator;

#include "hashmap_iter.h"

struct HashTable {
public:
    enum KeyType {
        WKN,
        NAME
    };

private:
    std::vector<Bucket> m_nameTable;

    std::vector<Bucket> m_wknTable;

    Bucket m_invalid;

    template <KeyType keyType>
    inline std::string& getKey(Share& share) const;

    template <KeyType keyType>
    inline std::vector<Bucket>& getTable();

    template <KeyType keyType>
    Bucket* insert_side(Share* s);

    inline hash_t make_hash(std::string& str) const
    {
        hash_t hash_value = 0, base = 127;
        for (auto c : str) {
            hash_value = (hash_value * base + c);
        }
        return hash_value;
    }

    template <KeyType keyType>
    std::unique_ptr<Share> remove(std::string& key)
    {
        for (auto& bucket : iter<keyType>(key)) {

            if (bucket.other == nullptr)
                return nullptr;

            if (getKey<keyType>(*bucket.data) == key) {
                bucket.other->other = &m_invalid;
                bucket.other = &m_invalid;
                auto tmp = bucket.data;
                bucket.data = nullptr;
                return std::unique_ptr<Share>(tmp);
            }
        }
    }

public:
    explicit HashTable(size_t size)
        : m_nameTable(size)
        , m_wknTable(size)
    {
    }

    template <KeyType keyType>
    hashiter iter(std::string& key)
    {
        return hashiter(make_hash(key), getTable<keyType>());
    }

    Share* insert(Share&& share);

    void print();

    template <KeyType keyType>
    Bucket* get_for_key(std::string& str);

    Share* get_by_name(std::string& str)
    {
        auto tmp = get_for_key<NAME>(str);
        if (tmp)
            return tmp->data;
        else
            return nullptr;
    }

    Share* get_by_wkn(std::string& str)
    {
        auto tmp = get_for_key<WKN>(str);
        if (tmp)
            return tmp->data;
        else
            return nullptr;
    }

    std::unique_ptr<Share> remove_by_name(std::string& str)
    {
        return std::move(remove<NAME>(str));
    }

    std::unique_ptr<Share> remove_by_wkn(std::string& str)
    {
        return std::move(remove<WKN>(str));
    }
};

#include "hashmap_impl.h"

#endif //DEFINITIVEHASH_HASHMAP_H
