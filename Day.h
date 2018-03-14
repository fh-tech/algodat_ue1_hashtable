//
// Created by vik on 11.03.18.
//

#ifndef HASHTABLE_DAY_H
#define HASHTABLE_DAY_H

#include <iostream>

class Day {
    uint32_t date; //unix timestamp ?   oder doch lieber als string?
    float open;
    float high;
    float low;
    float close;
    float adj_close;
    uint32_t volume;
public:
    Day(uint32_t date, float open, float high, float low, float close, float adj_close, uint32_t volume)
    :date(date),
    open(open),
    high(high),
    low(low),
    close(close),
    adj_close(adj_close),
    volume(volume)
    {}
};


#endif //HASHTABLE_DAY_H
