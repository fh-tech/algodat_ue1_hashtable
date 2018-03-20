//
// Created by vik on 13.03.18.
//

#include <gtest/gtest.h>
#include "../hashmap.h"
#include "utils.h"
#include <nlohmann/json.hpp>


TEST(hashtable, test1) {
    HashTable hashtable = HashTable(1000);

    Share share = Share{"Microsoft", "MSFT", "1234"};

    hashtable.insert(std::move(share));

    std::string name = "Microsoft";
    Share * s_name = hashtable.get_by_name(name);

    std::string id = "MSFT";
    Share * s_id = hashtable.get_by_id(id);



    std::cout << s_name << std::endl;
    std::cout << s_id << std::endl;



    Share s = Share{"Microsoft", "MSFT", "1234"};


    ASSERT_EQ(s, *s_id);
    ASSERT_EQ(s, *s_id);
    ASSERT_EQ(s_name, s_id);
}

TEST(hashtable, test2){

    HashTable hashtable = HashTable(4);

    Share s0 = {"a", "A", "a"};
    Share s1 = {"b", "B", "b"};
    Share s2 = {"c", "C", "c"};
    Share s3 = {"v", "v", "v"};

    ASSERT_TRUE(hashtable.insert(std::move(s0)));
    ASSERT_TRUE(hashtable.insert(std::move(s1)));
    ASSERT_TRUE(hashtable.insert(std::move(s2)));

}

TEST(json, day_serialier){

    Day d = random_day();
    json j = d;
    std::cout  << j << std::endl;

    Day d2 = j;

    ASSERT_EQ(d, d2);
}

TEST(json, share_serialier){

    Share s = random_share();
    json j = s;
    std::cout  << j << std::endl;

    Share s2 = j;

    ASSERT_EQ(s, s2);
}

TEST(json, hashtable_serialier){

    auto shares = random_shares(10);
    HashTable ht(15);

    for(auto&& s: shares)
        ht.insert(std::move(s));

    json j = ht;

    std::cout << j;

    HashTable ht2 = j;

    ASSERT_EQ(ht, ht2);
}

