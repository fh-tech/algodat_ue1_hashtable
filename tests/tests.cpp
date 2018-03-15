//
// Created by vik on 13.03.18.
//

#include <gtest/gtest.h>
#include "../Hashtable.h"

TEST(hashtable, test1) {
    Hashtable hashtable = Hashtable(1000);
    std::string name = "Microsoft";
    std::string wkn = "1234";
    std::string id = "MSF";
    Share *share = new Share(name,wkn,id);

    hashtable.insert(share);

    Bucket * b_name = hashtable.search_byName(name);
    Bucket * b_wkn = hashtable.search_byWKN(wkn);

    std::cout << b_name->data << std::endl;
    std::cout << b_wkn->data << std::endl;
    std::cout << share << std::endl;

    ASSERT_EQ(share, b_name->data);
    ASSERT_EQ(share, b_wkn->data);
    ASSERT_EQ(b_name->data, b_wkn->data);
}