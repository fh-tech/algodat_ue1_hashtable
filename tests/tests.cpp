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
    ASSERT_EQ(share->get_name(), name);
    ASSERT_EQ(share->get_wkn(), wkn);

    hashtable.insert(share);

    Share * test1 = hashtable.search_byName(name);
    Share * test2 = hashtable.search_byWKN(wkn);

    ASSERT_EQ(share, test1);
    ASSERT_EQ(share, test2);
    ASSERT_EQ(test1, test2);
}