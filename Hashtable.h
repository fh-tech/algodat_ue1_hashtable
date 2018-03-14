//
// Created by vik on 11.03.18.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include <iostream>
#include <array>
#include <vector>
#include "Share.h"

typedef uint32_t hash_t;

struct Bucket {
    Bucket * other;
    Share * data;
};

struct quadraticProbingIterator {

private:
    size_t size;
    size_t i = 0;
    Bucket * first;
    hash_t hash;

public:
    quadraticProbingIterator(std::vector<Bucket> &buckets, hash_t hash)
            : size(buckets.capacity()),
              first(&buckets[0]),
              hash(hash)
    {}

    Bucket * begin() {
        return first + hash % size;
    }

    constexpr Bucket * end() {
        return nullptr;
    }

    Bucket* operator ++() {
        ++i;
        return operator*();
    }

    Bucket* operator *() {
        if(i < size) {
            return first + ((hash + i * i) % size);
        } else {
            return nullptr;
        }
    }
};


class Hashtable {
public:
    Hashtable(uint16_t size)
            : table_size(size),
              shares_name(size),
              shares_wkn(size) {}

    void insert(Share *share) {
        std::string share_name = share->get_name();
        std::string share_wkn = share->get_wkn();

        uint32_t hash_name = makeHash(share_name);
        uint32_t hash_wkn = makeHash(share_wkn);

        Bucket *bucket_name = insertWithHash(hash_name, share, 0, 1, shares_name); //return value is pointer to the share
        Bucket *bucket_wkn = insertWithHash(hash_wkn, share, 0, 1, shares_wkn);

        bucket_name->other = bucket_wkn;
        bucket_wkn->other = bucket_name;
    }

    //specify printAll<1>()  or printAll<0>()
    template<int j>
    void printAll() const {
        for (auto i = 0; i < table_size; i++) {
            Share *share;
            if constexpr (j == 1) {
                share = shares_name[i].data;
            } else {
                share = shares_wkn[i].data;
            }
            std::cout << i << ": " << share << std::endl;
        }
    }

    Bucket* search_byName(std::string &key) { //no const because possible that getting changes
        uint32_t hash_name = makeHash(key);
        Bucket* bucket = getWithHash<1>(hash_name, key, 0, 1, shares_name);
        return bucket;
    }

    Bucket* search_byWKN(std::string &key) {
        uint32_t hash_wkn = makeHash(key);
        Bucket* bucket = getWithHash<0>(hash_wkn, key, 0, 1, shares_wkn);
        return bucket;
    }

    // remove only possible by name!! //FIXME: need cleanup here but is it possible see (my block haha)
    void removeShare(std::string &key) {
        hash_t hash_name = makeHash(key);
        Bucket* bucket_name = getWithHash<1>(hash_name, key, 0, 1, shares_name);
        Bucket* bucket_wkn = bucket_name->other;
        if(bucket_name->data->get_name() == key && bucket_wkn->data->get_wkn() == bucket_name->data->get_wkn()) {       //FIXME: keep sanity check?
            Share * data = bucket_name->data;

            bucket_name->other = NULL;
            bucket_wkn->other = NULL;
            bucket_name->data = NULL;
            bucket_wkn->data = NULL;

            delete data;
        } else {
            exit(1);      //FIXME: should we break the program in this case?
        }
    }


private:
    const uint16_t table_size;
    std::vector<Bucket> shares_name; //main store which stores the pointer to the share
    std::vector<Bucket> shares_wkn; //side store which stores a pointer to the pointer to the share


    Bucket* insertWithHash(hash_t hash, Share *share, uint8_t i, uint8_t tries, std::vector<Bucket> &table) {
        uint16_t pos = (hash + (i * i)) % table_size;
        if (table[pos].data == nullptr) {
            table[pos].data = share;
            return &table[pos];
        } else {
            if (tries == table_size + 1) {
                exit(1); //TODO think about abort condition
            } else {
                insertWithHash(hash, share, i + 1, tries + 1, table);
            }
        }
    }

    // with i = 1 you search with name
    // with i = 0 you search with wkn
    template<int j>
    Bucket* getWithHash(hash_t hash, std::string key, uint8_t i, uint16_t tries, std::vector<Bucket> &table) {
        Bucket bucket = table[(hash + (i * i)) % table_size];
        if (bucket.data != nullptr) {
            auto flag = false;
            if constexpr (j == 1) {
                flag = key == bucket.data->get_name();
            } else {
                flag = key == bucket.data->get_wkn();
            }

            if (flag) { // flag is true so we found the share we wanted
                Bucket *buck = &bucket;
                return buck;           // FIXME: why doesnt return &bucket work ?
            } else {  // if its the wrong share --> share with same hash keep searching
                getWithHash<j>(hash, key, i + 1, tries + 1, table);
            }
        } else {
            // first empty field --> not in table
            return nullptr;
        }
    }


    hash_t makeHash(std::string &key) {
        hash_t hash_value = 0, base = 127;
        for (auto i : key) {
            hash_value = (hash_value * base + i);
        }
        return hash_value;
    }

};


#endif //HASHTABLE_HASHTABLE_H
