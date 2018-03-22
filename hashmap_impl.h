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
    if constexpr (keyType == HashTable::ID) {
        return share.id;
    } else {
        return share.name;
    }
}

template <HashTable::KeyType keyType>
constexpr std::vector<Bucket>& HashTable::getTable()
{
    if constexpr (keyType == ID) {
        return m_idTable;
    } else {
        return m_nameTable;
    }
}

template <HashTable::KeyType keyType>
Bucket* HashTable::insert_side(std::string& s)
{

    for (Bucket& bucket : iter<keyType>(s)) {

        if (bucket.data == nullptr
            || bucket.other == &m_invalid
            || getKey<keyType>(*bucket.data) == s) {

            return &bucket;
        }
    }

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
    auto sh_ptr = new Share(std::move(share));

    if (auto id_ptr = insert_side<ID>(sh_ptr->id)) {
        if(auto name_ptr = insert_side<NAME>(sh_ptr->name)) {

            if(id_ptr->data || name_ptr->data){
                if(   id_ptr->other == name_ptr
                   && name_ptr->other == id_ptr){
                    delete id_ptr->data;
                    id_ptr->data = sh_ptr;
                    name_ptr->data = sh_ptr;
                    return sh_ptr;
                } else return nullptr;
            }

            name_ptr->data = sh_ptr;
            id_ptr->data = sh_ptr;

            name_ptr->other = id_ptr;
            id_ptr->other = name_ptr;

            return sh_ptr;
        } else {
            delete id_ptr->data;
            id_ptr->other = nullptr;
            return nullptr;
        }
    } else {
        delete sh_ptr;
        return nullptr;
    }
}

template <HashTable::KeyType keyType>
Bucket* HashTable::get_for_key(std::string& key)
{

    Bucket* toSwitch = nullptr;

    for (auto& bucket : iter<keyType>(key)) {
        if (bucket.other == nullptr) {

            return nullptr;

        }else if (bucket.other == &m_invalid) {

            toSwitch = &bucket;

        }else if (getKey<keyType>(*bucket.data) == key) {

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

hash_t HashTable::make_hash(std::string &str) const
{
    hash_t hash_value = 0, base = 127;
    for (auto c : str) {
        hash_value = (hash_value * base + c);
    }
    return hash_value;
}


template<HashTable::KeyType keyType>
std::unique_ptr<Share> HashTable::remove(std::string &key)
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

bool HashTable::operator==(const HashTable &other) const {

    std::vector<Share*> thisbucketref{};
    std::vector<Share*> otherbucketref{};

    for(auto b: other.m_idTable){
        if(b.other != nullptr && b.other != &other.m_invalid){
            otherbucketref.push_back(b.data);
        }
    }

    for(auto b: m_idTable){
        if(b.other != nullptr && b.other != &m_invalid){
            thisbucketref.push_back(b.data);
        }
    }

    return std::is_permutation(thisbucketref.begin(), thisbucketref.end(),
                               otherbucketref.begin(), otherbucketref.end(),
                                [](auto s1, auto s2){ return *s1 == *s2;});
}

#endif //DEFINITIVEHASH_HASHMAP_IMPL_H
