//
// Created by daniel on 16.03.18.
//

#ifndef HASHTABLE_UTILS_H
#define HASHTABLE_UTILS_H

#include <string>
#include <random>

#include "../Share.h"

static std::default_random_engine rnd{std::random_device{}()};

std::string random_str(std::string::size_type len){
    static auto& chars = "0123456789ABCDEF";


    static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chars)-2);

    std::string s;
    s.reserve(len);

    while (len--){
        s += chars[pick(rnd)];
    }
    return s;
}

Day random_day(){
    static std::normal_distribution<float> rng{100, 30};
    return Day{
            (uint32_t) rng(rnd),
            rng(rnd),
            rng(rnd),
            rng(rnd),
            rng(rnd),
            rng(rnd),
            (uint32_t) rng(rnd),
    };
}


Share random_share() {
    Share s {
            std::move(random_str(20)),
            std::move(random_str(20)),
            std::move(random_str(20)),

    };

    for(auto& d: s.days)
        d = random_day();

    return s;
}



std::vector<Share> random_shares(size_t len){
    std::vector<Share> vec;
    while (len--)
        vec.push_back(random_share());

    return vec;
}


#endif //HASHTABLE_UTILS_H
