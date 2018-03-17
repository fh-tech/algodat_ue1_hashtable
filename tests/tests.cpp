//
// Created by vik on 13.03.18.
//

#include <gtest/gtest.h>
#include "../hashmap.h"

TEST(hashtable, test1) {
    HashTable hashtable = HashTable(1000);
    std::string name = "Microsoft";
    std::string code = "1234";
    std::string id = "MSF";
    Share share = Share{"Microsoft", "1234", "MSF"};

    hashtable.insert(std::move(share));

    Share * s_name = hashtable.get_by_name(name);
    Share * s_code = hashtable.get_by_code(code);

    std::cout << s_name << std::endl;
    std::cout << s_code << std::endl;

    Share s = Share{"Microsoft", "1234", "MSF"};

    if(s == *s_code){

    }

    ASSERT_EQ(s, *s_code);
    ASSERT_EQ(s, *s_code);
    ASSERT_EQ(s_name, s_code);
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


