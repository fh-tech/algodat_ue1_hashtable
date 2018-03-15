//
// Created by daniel on 15.03.18.
//
#ifndef DEFINITIVEHASH_HASHMAP_IMPL_H
#define DEFINITIVEHASH_HASHMAP_IMPL_H

#include "hashmap.h"
#include <iomanip>

template <HashTable::KeyType keyType>
inline std::string& HashTable::getKey(Share& share) const
{
    if constexpr (keyType == HashTable::WKN) {
        return share.wkn;
    } else {
        return share.name;
    }
}

template <HashTable::KeyType keyType>
inline std::vector<Bucket>& HashTable::getTable()
{
    if constexpr (keyType == WKN) {
        return m_wknTable;
    } else {
        return m_nameTable;
    }
}

template <HashTable::KeyType keyType>
Bucket* HashTable::insert_side(Share* s)
{

    for (Bucket& bucket : iter<keyType>(getKey<keyType>(*s))) {

        if (bucket.data == nullptr
            || bucket.other == &m_invalid
            || getKey<keyType>(*bucket.data) == getKey<keyType>(*s)) {

            bucket.data = s;
            return &bucket;
        }
    }

    return nullptr;
}

void HashTable::print()
{
    for (auto& b : m_wknTable) {
        std::cout << std::setw(14) << b.data << "  : " << (b.data ? b.data->name : "") << '\n';
    }
    std::cout << std::endl;
}

Share* HashTable::insert(Share&& share)
{
    auto sh_ptr = new Share(std::move(share));

    if (auto wkn_ptr = insert_side<WKN>(sh_ptr)) {
        auto name_ptr = insert_side<NAME>(sh_ptr);
        wkn_ptr->other = name_ptr;
        name_ptr->other = wkn_ptr;

        return sh_ptr;
    }
    return nullptr;
}

template <HashTable::KeyType keyType>
Bucket* HashTable::get_for_key(std::string& key)
{

    Bucket* toSwitch = nullptr;

    for (auto& bucket : iter<keyType>(key)) {
        if (bucket.other == nullptr)
            return nullptr;

        if (bucket.other == &m_invalid)
            toSwitch = &bucket;

        if (getKey<keyType>(*bucket.data) == key) {
            if (toSwitch) {
                std::swap(*toSwitch, bucket);
                toSwitch->other->other = toSwitch;
                return toSwitch;
            } else {
                return &bucket;
            }
        };
    }
}

#endif //DEFINITIVEHASH_HASHMAP_IMPL_H
