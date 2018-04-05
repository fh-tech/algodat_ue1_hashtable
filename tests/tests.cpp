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
    Share *s_name = hashtable.get_by_name(name);

    std::string id = "MSFT";
    Share *s_id = hashtable.get_by_id(id);


    std::cout << s_name << std::endl;
    std::cout << s_id << std::endl;


    Share s = Share{"Microsoft", "MSFT", "1234"};


    ASSERT_EQ(s, *s_id);
    ASSERT_EQ(s, *s_id);
    ASSERT_EQ(s_name, s_id);
}

TEST(hashtable, test2) {

    HashTable hashtable = HashTable(4);

    Share s0 = {"a", "A", "a"};
    Share s1 = {"b", "B", "b"};
    Share s2 = {"c", "C", "c"};
    Share s3 = {"v", "v", "v"};

    ASSERT_TRUE(hashtable.insert(std::move(s0)));
    ASSERT_TRUE(hashtable.insert(std::move(s1)));
    ASSERT_TRUE(hashtable.insert(std::move(s2)));

}

TEST(json, day_serialier) {

    Day d = random_day();
    json j = d;
    std::cout << j << std::endl;

    Day d2 = j;

    ASSERT_EQ(d, d2);
}

TEST(json, share_serialier) {

    Share s = random_share();
    json j = s;
    std::cout << j << std::endl;

    Share s2 = j;

    ASSERT_EQ(s, s2);
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


//together 30 elements
TEST(update_days_check, import_utils) {
    std::array<Day, 30> target{};
    std::array<Day, 30> source{};
    std::array<uint32_t, 30> source_dates{30, 28, 27, 22, 21, 20, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};
    std::array<uint32_t, 30> target_dates{29, 26, 25, 24, 23, 19, 18, 17, 16, 15, 14, 13, 12, 2, 1, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};

    int i = 0;
    for (auto date: target_dates) {
        target[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }
    i = 0;
    for (auto date: source_dates) {
        source[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }

    updateDays(target, source);


    std::array<uint32_t, 30> result_dates{30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                                          11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};


    //check if target dates equal the expected result dates
    for (int j = 0; j < 30; j++) {
        ASSERT_EQ(target[j].date, result_dates[j]);
    }
}

//target empty (first import)
TEST(update_days_check2, import_utils) {
    std::array<Day, 30> target{};
    std::array<Day, 30> source{};
    std::array<uint32_t, 30> source_dates{30, 28, 27, 22, 21, 20, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};
    std::array<uint32_t, 30> target_dates{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};

    int i = 0;
    for (auto date: target_dates) {
        target[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }
    i = 0;
    for (auto date: source_dates) {
        source[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }

    updateDays(target, source);


    std::array<uint32_t, 30> result_dates{30, 28, 27, 22, 21, 20, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};

    //check if target dates equal the expected result dates
    for (int j = 0; j < 30; j++) {
        ASSERT_EQ(target[j].date, result_dates[j]);
    }
}

//more than 31 elements together that would fit
TEST(update_days_check3, import_utils) {
    std::array<Day, 30> target{};
    std::array<Day, 30> source{};
    std::array<uint32_t, 30> source_dates{30, 28, 27, 22, 21, 20, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0};
    std::array<uint32_t, 30> target_dates{31, 29, 26, 25, 24, 23, 19, 18, 17, 16, 15, 14, 13, 12, 2, 1, 0, 0, 0, 0, 0,
                                          0, 0,
                                          0, 0, 0, 0, 0, 0};

    int i = 0;
    for (auto date: target_dates) {
        target[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }
    i = 0;
    for (auto date: source_dates) {
        source[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }

    updateDays(target, source);

    // does not use 1 because it is the oldest that does not fit in anymore
    std::array<uint32_t, 30> result_dates{31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                                          12,
                                          11, 10, 9, 8, 7, 6, 5, 4, 3, 2};


    //check if target dates equal the expected result dates
    for (int j = 0; j < 30; j++) {
        ASSERT_EQ(target[j].date, result_dates[j]);
    }
}

// both full but new dates are one day newer (complete new array should be used)   2 dates duplicate (30)
TEST(update_days_check4, import_utils) {
    std::array<Day, 30> target{};
    std::array<Day, 30> source{};
    std::array<uint32_t, 30> source_dates{31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                                          12, 11,
                                          10, 9, 8, 7, 5, 4, 3, 2, 1};
    std::array<uint32_t, 30> target_dates{30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                                          11,
                                          10, 9, 8, 7, 5, 4, 3, 2, 1, 0};

    int i = 0;
    for (auto date: target_dates) {
        target[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }
    i = 0;
    for (auto date: source_dates) {
        source[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }

    updateDays(target, source);

    // does not use 1 because it is the oldest that does not fit in anymore
    std::array<uint32_t, 30> result_dates{31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                                          12, 11,
                                          10, 9, 8, 7, 5, 4, 3, 2, 1};


    //check if target dates equal the expected result dates
    for (int j = 0; j < 30; j++) {
        ASSERT_EQ(target[j].date, result_dates[j]);
    }
}

//target array completely empty
TEST(update_days_check5, import_utils) {
    std::array<Day, 30> target{};
    std::array<Day, 30> source{};
    std::array<uint32_t, 30> source_dates{31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                                          12, 11,
                                          10, 9, 8, 7, 5, 4, 3, 2, 1};
    std::array<uint32_t, 30> target_dates{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0};

    int i = 0;
    for (auto date: target_dates) {
        target[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }
    i = 0;
    for (auto date: source_dates) {
        source[i++] = Day{date, 0, 0, 0, 0, 0, 0};
    }

    updateDays(target, source);

    // does not use 1 because it is the oldest that does not fit in anymore
    std::array<uint32_t, 30> result_dates{31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                                          12, 11,
                                          10, 9, 8, 7, 5, 4, 3, 2, 1};


    //check if target dates equal the expected result dates
    for (int j = 0; j < 30; j++) {
        ASSERT_EQ(target[j].date, result_dates[j]);
    }
}