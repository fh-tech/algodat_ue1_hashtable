//
// Created by daniel on 15.03.18.
//
#ifndef DEFINITIVEHASH_HASHMAP_H
#define DEFINITIVEHASH_HASHMAP_H

#include "Share.h"
#include <bits/unique_ptr.h>
#include <vector>

using hash_t = uint32_t;

/**
 * Bucket type containing a pointer to the actual data
 * and a pointer into the other bucket table where the corresponding Bucket is located
 * Both, this Bucket and other, point to the same data
 */
struct Bucket {
    Share* data{ nullptr };
    Bucket* other{ nullptr };
};

#include "hashmap_iter.h"

/** \brief A hashtable for saving Share data.
 *
 * A hashtable implementation using quadratic probing for collision avoidance.
 * The main data structure consists of two std::vector, where each contains a list of buckets.
 * A bucket is a simple struct, which holds both a pointer into the other table and a pointer to the actual element.
 *
 * This hashtable uses quadratic probing in case of a hash collision and provides an easy c++-iterator interface to generate
 * a sequence of Buckets for a specific hash.
 *
 * This table supports inserting, removing, and searching Share objects. The search and delete is implemented to support searching
 * and deleting via two different keys.
 *
 * On delete a bucket is not removed, but set invalid. An invalid bucket is ignored when searching and overwritten when inserting.
 * To combat performance degradation, when a lot of elements are inserted and then removed, during a search the first invalid
 * element is marked and once the searched for element is found the table switches the invalid with the found element.
 *
 */

class HashTable {
public:
    /**
     * The two key types of a share object.
     * Multiple methods are templated to provide different function depending which KeyType is used.
     */

    enum KeyType {
        ID,
        NAME
    };

    /**
     * NameTable: bucket list for share entries where the name is used as key
     */
    std::vector<Bucket> m_nameTable{};
    /**
     * IdTable: bucket list for share entries where the name is used as key
     */
    std::vector<Bucket> m_idTable{};
    /**
     * InvalidBucket: A bucket, whose other pointer points to this element is considered invalid.
     * Has no other use than to be pointed at.
     */
    Bucket m_invalid{};

private:
    /**
     * Selects the correct key value from a share given a keytype
     * @tparam keyType
     * @param share
     * @return a reference to the key value
     */
    template <KeyType keyType>
    inline std::string& getKey(Share& share) const;
    /**
     * Selects the correct table given a keytype
     * @tparam keyType
     * @return the internal bucket table corresponding to the keytype
     */
    template <KeyType keyType>
    constexpr std::vector<Bucket>& getTable();

    /**
     * Returns the next valid bucket for a given key and keytype where a new Share can be inserted
     * The returned bucket is either invalid or empty
     * @tparam keyType
     * @param s
     * @return returns a pointer to a bucket where a new share can be inserted or null if no empty or invalid bucket was found
     */
    template <KeyType keyType>
    Bucket* insert_side(std::string& s);

    /**
     * Generates a new hash for a given string
     * @param str A string
     * @return a hash value
     */
    inline hash_t make_hash(std::string& str) const;

    /**
     * Searches and removes (sets invalid) a Share by key
     * @tparam keyType The keytype of the key
     * @param key The key to search for
     * @return a unique_ptr to the removed share or null if no such share was found
     */
    template <KeyType keyType>
    std::unique_ptr<Share> remove(std::string& key);

public:
    HashTable() = default;
    /**
     * Constructs a new Hashtable with a specified maximal capacity.
     * Every bucket is initialized to empty.
     * @param size capacity of the hashtable
     */
    explicit HashTable(size_t size)
        : m_nameTable(size)
        , m_idTable(size)
    {
    }

    /**
     *  Makes sure every remaining heap-allocated Share is deleted when this hashtable runs out of scope.
     */
    ~HashTable()
    {
        for (auto& b : m_idTable) {
            if (b.data) {
                delete b.data;
            }
        }
    }
    /**
     * Provides an easy iterator interface for finding new elements using quadratic probing
     * @tparam keyType signals which type of key the @key parameter is
     * @param key The key for which a Quadratic probing sequence is generated
     * @return a HashIter object
     */
    template <KeyType keyType>
    hashiter iter(std::string& key)
    {
        return hashiter(make_hash(key), getTable<keyType>());
    }

    /**
     * Takes ownership of a Share and inserts it into the hashtable
     * @param share Share to insert
     * @return A pointer into the hashtable pointing to the inserted share or null if the insert failed
     */
    Share* insert(Share&& share);

    void print();

    /**
     * Searches the Hashtable for the string
     * @tparam keyType key type of @str
     * @param str the key
     * @return a Pointer to the bucket containing the searched for Share
     */
    template <KeyType keyType>
    Bucket* get_for_key(std::string& key);

    /**
     * Searches for a share with the specified name
     * @param str the name of the share
     * @return Returns a pointer to the share or null if it is not found
     */
    Share* get_by_name(std::string& str)
    {
        auto tmp = get_for_key<NAME>(str);
        if (tmp)
            return tmp->data;
        else
            return nullptr;
    }

    /**
     * Searches for a share with the specified id
     * @param str the id of the share
     * @return Returns a pointer to the share or null if it is not found
     */
    Share* get_by_id(std::string& str)
    {
        auto tmp = get_for_key<ID>(str);
        if (tmp)
            return tmp->data;
        else
            return nullptr;
    }

    /**
     * Equality operator for a Hashtable
     * Two hashtables are considered equal if they contain the same share objects
     * Note this operation is expensive
     * @param other
     * @return true if other is equal to this
     */
    bool operator==(const HashTable& other) const;

    /**
     * Removes an element from the hashtable
     * The actual Bucket is not removed, because this would entail moving every bucket in the probing sequence over by one,
     * but set invalid
     * @param str the name of the share to remove
     * @return A unique_ptr to the deleted share or null if the share was not found
     */
    std::unique_ptr<Share> remove_by_name(std::string& str)
    {
        return remove<NAME>(str);
    }

    /**
     * Removes an element from the hashtable
     * The actual Bucket is not removed, because this would entail moving every bucket in the probing sequence over by one,
     * but set invalid
     * @param str the id of the share to remove
     * @return A unique_ptr to the deleted share or null if the share was not found
     */
    std::unique_ptr<Share> remove_by_id(std::string& str)
    {
        return remove<ID>(str);
    }
};

#include "hashmap_impl.h"

void from_json(const json& j, HashTable& ht)
{
}

static void to_json(json& j, const HashTable& ht)
{
    j["capacity"] = ht.m_idTable.size();
    std::vector<Share> shs{};
    for (auto& b : ht.m_idTable) {
        if (b.other != nullptr && b.other != &ht.m_invalid) {
            if (b.data)
                shs.push_back(*b.data);
        }
    }
    j["elements"] = shs;
}

#endif //DEFINITIVEHASH_HASHMAP_H
