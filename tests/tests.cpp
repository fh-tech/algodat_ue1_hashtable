//
// Created by vik on 13.03.18.
//

#include <gtest/gtest.h>
#include "../hashmap.h"

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


