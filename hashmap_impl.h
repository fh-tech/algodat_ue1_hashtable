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
    if constexpr (keyType == HashTable::CODE) {
        return share.code;
    } else {
        return share.name;
    }
}

template <HashTable::KeyType keyType>
inline std::vector<Bucket>& HashTable::getTable()
{
    if constexpr (keyType == CODE) {
        return m_codeTable;
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
    for (auto& b : m_codeTable) {
        std::cout << std::setw(14) << b.data << "  : " << (b.data ? b.data->name : "") << '\n';
    }
    std::cout << std::endl;
}

Share* HashTable::insert(Share&& share)   //REVIEW what exactly is Share&&
{
    auto sh_ptr = new Share(std::move(share));   //REVIEW is that a unique_ptr now?   why move and why make new Share at all

    if (auto code_ptr = insert_side<CODE>(sh_ptr)) {     // is pointer to bucket or nullptr
        auto name_ptr = insert_side<NAME>(sh_ptr);     // another Bucket *
        code_ptr->other = name_ptr;                     // REVIEW what if first insert is successful but second into the other vector not (wont happen probably)
        name_ptr->other = code_ptr;                      // now point at each other

        return sh_ptr;
    }
    return nullptr;
}

template <HashTable::KeyType keyType>          //REVIEW why not only KeyType keytype --> we are in class should work
Bucket* HashTable::get_for_key(std::string& key)
{

    Bucket* toSwitch = nullptr;

    for (auto& bucket : iter<keyType>(key)) {
        if (bucket.other == nullptr)      //REVIEW doesnt matter if you check other or data
            return nullptr;

        if (bucket.other == &m_invalid)     //REVIEW again it should not matter if we use other or data? because when setting invalid we remove link to other table?
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
