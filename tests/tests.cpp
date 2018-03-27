//
// Created by vik on 13.03.18.
//

#include <gtest/gtest.h>
#include "../hashmap.h"
#include "utils.h"
#include "../main_utils.h"
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

TEST(ht_insert, hashtable_no_leak_when_double_insert) {
    HashTable ht{100};

    auto s = random_share();
    auto sc = s;

    auto s1 = random_share();
    auto s1c = s1;

    auto s2 = random_share();
    auto s2c = s2;

    auto s3 = random_share();
    auto s3c = s3;

    ht.insert(std::move(s));
    ht.insert(std::move(s1));
    ht.insert(std::move(s2));

    sc.id = s2c.id;

    ASSERT_EQ(ht.insert(std::move(sc)), nullptr);

}


TEST(update_days, import_utils) {
    HashTable ht{100};
    std::ifstream input_file = std::ifstream("/home/vik/Repos/ue1_hashtable/tests/data/MSFT.csv");
    std::array<Day, 30> target = import_fromFile(input_file);
    input_file = std::ifstream("/home/vik/Repos/ue1_hashtable/tests/data/MSFT2.csv");
    std::array<Day, 30> source = import_fromFile(input_file);

    updateDays(target, source);   //problem that i could fill all 30 days but update cant do it

    int i = 0;


}
