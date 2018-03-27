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
    Share* data{ nullptr };
    Bucket* other{ nullptr };
};

#include "hashmap_iter.h"

class HashTable {
public:
    enum KeyType {
        ID,
        NAME
    };

    std::vector<Bucket> m_nameTable{};

    std::vector<Bucket> m_idTable{};

    Bucket m_invalid{};

private:

    template <KeyType keyType>
    inline std::string& getKey(Share& share) const;

    template <KeyType keyType>
    constexpr std::vector<Bucket>& getTable();

    template <KeyType keyType>
    Bucket* insert_side(std::string& s);

    inline hash_t make_hash(std::string& str) const;

    template <KeyType keyType>
    std::unique_ptr<Share> remove(std::string& key);

public:

    HashTable() = default;

    explicit HashTable(size_t size)
        : m_nameTable(size)
        , m_idTable(size)
    {
    }

    ~HashTable(){
        for(auto& b: m_idTable){
            if(b.data)
            {
                delete b.data;
            }
        }
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

    bool operator==(const HashTable& other) const;

    std::unique_ptr<Share> remove_by_name(std::string& str)
    {
        return remove<NAME>(str);
    }

    std::unique_ptr<Share> remove_by_id(std::string& str)
    {
        return remove<ID>(str);
    }

};


#include "hashmap_impl.h"

void from_json(const json& j, HashTable& ht){

}

static void to_json(json& j, const HashTable& ht){
    j["capacity"] = ht.m_idTable.size();
    std::vector<Share> shs{};
    for(auto& b: ht.m_idTable){
        if(b.other != nullptr && b.other != &ht.m_invalid){
            if(b.data)
                shs.push_back(*b.data);

        }
    }
    j["elements"] = shs;
}

#endif //DEFINITIVEHASH_HASHMAP_H
