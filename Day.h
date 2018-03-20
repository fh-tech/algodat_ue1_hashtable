//
// Created by vik on 11.03.18.
//

#ifndef HASHTABLE_DAY_H
#define HASHTABLE_DAY_H

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Day {
public:

    uint32_t date;
    uint32_t volume;

    float open;
    float high;
    float low;
    float close;
    float adj_close;

    Day() = default;

    Day(uint32_t date, float open, float high, float low, float close,
        float adj_close, uint32_t volume)
            : date(date), open(open), high(high), low(low), close(close), adj_close(adj_close), volume(volume) {
    }

    bool operator==(const Day &other) const {
        return open == other.open
               && high == other.high
               && low == other.low
               && close == other.close
               && adj_close == other.adj_close
               && volume == other.volume;
    }
};

void to_json(json &j, const Day &day) {
    j = json{
            {"open",      day.open},
            {"date",      day.date},
            {"high",      day.high},
            {"low",       day.low},
            {"close",     day.close},
            {"adj_close", day.adj_close},
            {"volume",    day.volume}
    };
}

void from_json(const json &j, Day &d) {
    d.volume = j["volume"];
    d.adj_close = j["adj_close"];
    d.close = j["close"];
    d.open = j["open"];
    d.low = j["low"];
    d.high = j["high"];
    d.date = j["date"];
}

#endif // HASHTABLE_DAY_H
