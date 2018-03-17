//
// Created by daniel on 16.03.18.
//

#ifndef HASHTABLE_UTILS_H
#define HASHTABLE_UTILS_H

#include <string>
#include <random>

#include "../Share.h"

std::string random_str(std::string::size_type len){
    static auto& chars = "0123456789ABCDEF";

    thread_local static
    std::default_random_engine rnd{std::random_device{}()};

    thread_local static
    std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chars)-2);

    std::string s;
    s.reserve(len);

    while (len--){
        s += chars[pick(rnd)];
    }
    return s;
}

inline Share random_share() {
    return Share {
            std::move(random_str(5)),
            std::move(random_str(5)),
            std::move(random_str(5)),
    };
}

std::vector<Share> random_shares(size_t len){
    std::vector<Share> vec;
    while (len--)
        vec.push_back(random_share());

    return vec;
}


#endif //HASHTABLE_UTILS_H
