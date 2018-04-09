//
// Created by daniel on 15.03.18.
//
#ifndef DEFINITIVEHASH_HASHMAP_IMPL_H
#define DEFINITIVEHASH_HASHMAP_IMPL_H
/**
 * Implementation for hashmap.h
 */
#include "hashmap.h"
#include <iomanip>

template <HashTable::KeyType keyType>
inline std::string& HashTable::getKey(Share& share) const
{
    //constexpr because we know to compile time if we want id or name
    if constexpr (keyType == HashTable::ID) {
        return share.id;
    } else {
        return share.name;
    }
}

template <HashTable::KeyType keyType>
constexpr std::vector<Bucket>& HashTable::getTable()
{
    //constexpr because we know at compile time if we want id or name
    if constexpr (keyType == ID) {
        return m_idTable;
    } else {
        return m_nameTable;
    }
}

template <HashTable::KeyType keyType>
Bucket* HashTable::insert_side(std::string& s)
{
    //iterate over the probing sequence
    for (Bucket& bucket : iter<keyType>(s)) {
        //a bucket is valid for insertion if it is empty, invalid or if we want to overwrite the entry
        if (bucket.other == nullptr
            || bucket.other == &m_invalid
            || getKey<keyType>(*bucket.data) == s) {

            return &bucket;
        }
    }
    //no matching bucket was found in the probing sequence
    return nullptr;
}

void HashTable::print()
{
    for (auto& b : m_idTable) {
        std::cout << std::setw(14) << b.data << "  : " << (b.data ? b.data->name : "") << '\n';
    }
    std::cout << std::endl;
}

Share* HashTable::insert(Share&& share)
{
    //take ownership of share and allocate it on heap
    auto sh_ptr = new Share(std::move(share));
    //try to insert sh_ptr into both tables
    if (auto id_ptr = insert_side<ID>(sh_ptr->id)) {
        if (auto name_ptr = insert_side<NAME>(sh_ptr->name)) {

            //if the found positions are occupied delete contents and overwrite them
            if (id_ptr->data || name_ptr->data) {
                if (id_ptr->other == name_ptr
                    && name_ptr->other == id_ptr) {
                    delete id_ptr->data;
                    id_ptr->data = sh_ptr;
                    name_ptr->data = sh_ptr;
                    return sh_ptr;
                } else
                    return nullptr;
            }
            // insert new data into buckets and link them together
            name_ptr->data = sh_ptr;
            id_ptr->data = sh_ptr;

            name_ptr->other = id_ptr;
            id_ptr->other = name_ptr;

            return sh_ptr;
        } else {
            //we have only found a valid bucket on one side: abort
            delete id_ptr->data;
            id_ptr->other = nullptr;
            return nullptr;
        }
    } else {
        //we have not found a valid bucket: abort
        delete sh_ptr;
        return nullptr;
    }
}

template <HashTable::KeyType keyType>
Bucket* HashTable::get_for_key(std::string& key)
{
    //remember the first invalid bucket
    Bucket* toSwitch = nullptr;

    for (auto& bucket : iter<keyType>(key)) {

        if (bucket.other == nullptr) {
            //bucket is empty therefore probing sequence ends
            return nullptr;
        } else if (bucket.other == &m_invalid) {

            //if we have not found an invalid bucket remember the first
            if (!toSwitch)
                toSwitch = &bucket;

        } else if (getKey<keyType>(*bucket.data) == key) {
            //we found our key, if we have an invalid bucket swap them and return
            if (toSwitch) {
                std::swap(*toSwitch, bucket);
                toSwitch->other->other = toSwitch;
                return toSwitch;
            } else {
                return &bucket;
            }
        };
    }
    //no entry was found with our key
    return nullptr;
}

hash_t HashTable::make_hash(std::string& str) const
{
    hash_t hash_value = 0, base = 127;
    for (auto c : str) {
        hash_value = (hash_value * base + c);
    }
    return hash_value;
}

template <HashTable::KeyType keyType>
std::unique_ptr<Share> HashTable::remove(std::string& key)
{
    for (auto& bucket : iter<keyType>(key)) {

        //an empty bucket signals the end of the probing sequence
        if (bucket.other == nullptr)
            return nullptr;

        //ignore invalid bucket
        if (bucket.other == &m_invalid)
            continue;
        //found bucket, set other to invalid and move share out of table
        if (getKey<keyType>(*bucket.data) == key) {
            bucket.other->other = &m_invalid;
            bucket.other = &m_invalid;
            auto tmp = bucket.data;
            bucket.data = nullptr;
            return std::unique_ptr<Share>(tmp);
        }
    }
    return nullptr;
}

bool HashTable::operator==(const HashTable& other) const
{

    std::vector<Share*> thisbucketref{};
    std::vector<Share*> otherbucketref{};
    //collect all valid share pointer in both tables
    for (auto b : other.m_idTable) {
        if (b.other != nullptr && b.other != &other.m_invalid) {
            otherbucketref.push_back(b.data);
        }
    }

    for (auto b : m_idTable) {
        if (b.other != nullptr && b.other != &m_invalid) {
            thisbucketref.push_back(b.data);
        }
    }
    //if one pointer table is a permutation of the other they both have the same elements and therefore the hashtables are equal
    return std::is_permutation(thisbucketref.begin(), thisbucketref.end(),
        otherbucketref.begin(), otherbucketref.end(),
        [](auto s1, auto s2) { return *s1 == *s2; });
}

#endif //DEFINITIVEHASH_HASHMAP_IMPL_H
