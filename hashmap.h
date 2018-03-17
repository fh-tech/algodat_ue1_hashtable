//
// Created by daniel on 15.03.18.
//
#ifndef DEFINITIVEHASH_HASHMAP_H
#define DEFINITIVEHASH_HASHMAP_H

#include "Share.h"
#include <bits/unique_ptr.h>
#include <vector>

using hash_t = uint32_t;    //REVIEW: why not "typedef uint32_t hash_t" --> using reminds me of namespace

struct Bucket {
    Share* data = nullptr;       //REVIEW: why not {}
    Bucket* other = nullptr;
};

struct QuadraticProbingIterator; //REVIEW why ?

#include "hashmap_iter.h"

struct HashTable {        //REVIEW god damnit write class xD
public:
    enum KeyType {
        ID,
        NAME
    };

private:
    std::vector<Bucket> m_nameTable;

    std::vector<Bucket> m_idTable;     //REVIEW sollte jetzt vermutlich m_idTable sein

    Bucket m_invalid;

    template <KeyType keyType>
    inline std::string& getKey(Share& share) const;         //REVIEW inline because implementation in another .h file? --> why not all inline that have implementation elsewhere

    template <KeyType keyType>
    inline std::vector<Bucket>& getTable();

    template <KeyType keyType>
    Bucket* insert_side(Share* s);

    inline hash_t make_hash(std::string& str) const         //REVIEW left this method implementation here because it is short? it is not longer then others though think we should outsource them all
    {
        hash_t hash_value = 0, base = 127;
        for (auto c : str) {
            hash_value = (hash_value * base + c);
        }
        return hash_value;
    }

    template <KeyType keyType>
    std::unique_ptr<Share> remove(std::string& key) //REVIEW why does this return a unique_ptr
    {
        for (auto& bucket : iter<keyType>(key)) {

            if (bucket.other == nullptr)       // REVIEW should we not check data too for sanity?
                return nullptr;                // nothing to remove

            if (getKey<keyType>(*bucket.data) == key) {
                bucket.other->other = &m_invalid; //REVIEW shouldn`t we set bucket.other->data = nullptr;
                bucket.other = &m_invalid;
                auto tmp = bucket.data;
                bucket.data = nullptr;
                return std::unique_ptr<Share>(tmp);
            }
        }
    }

public:
    explicit HashTable(size_t size)   //REVIEW explicit to avoid conversions of size ?     and why pass size_t and not uint
        : m_nameTable(size)
        , m_idTable(size)
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

    Share* get_by_id(std::string& str)
    {
        auto tmp = get_for_key<ID>(str);
        if (tmp)
            return tmp->data;
        else
            return nullptr;
    }

    std::unique_ptr<Share> remove_by_name(std::string& str)    //REVIEW that shit breaks my brain
    {
        return std::move(remove<NAME>(str));
    }

    std::unique_ptr<Share> remove_by_id(std::string& str)
    {
        return std::move(remove<ID>(str));
    }
};

#include "hashmap_impl.h"       //REVIEW adding all methods into this header file like you normally would write them outside of class?

#endif //DEFINITIVEHASH_HASHMAP_H
