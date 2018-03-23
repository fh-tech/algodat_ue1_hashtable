//
// Created by vik on 11.03.18.
//

#ifndef HASHTABLE_SHARE_H
#define HASHTABLE_SHARE_H

#include "Day.h"
#include <array>
#include <cstdint>
#include <iostream>

struct Share {

    std::string name{};
    std::string wkn{}; // wertpapierkennnummer
    std::string id{}; // kürzel zum beispiel MSFT für microsoft aktie
    std::array<Day, 30> days{};


    Share() = default;

    Share(const Share &other) = default;

    Share(std::string &&name, std::string &&id, std::string &&wkn)
            : name(std::move(name)), wkn(std::move(wkn)), id(std::move(id)) {
    }

    Share(std::string &name, std::string &id, std::string &wkn, std::array<Day, 30> &days)
            : name(name),
              id(id),
              wkn(wkn),
              days(days) {

    }

    Share(Share &&other) = default;

    bool operator==(const Share &other) const {
        return name == other.name
               && id == other.id
               && wkn == other.wkn
               && days == other.days;
    }
};

void to_json(json &j, const Share &s) {
    j["name"] = s.name;
    j["wkn"] = s.wkn;
    j["id"] = s.id;
    j["days"] = s.days;
}

void from_json(const json &j, Share &s) {
    s.name = j.at("name").get<std::string>();
    s.id = j.at("id").get<std::string>();
    s.wkn = j.at("wkn").get<std::string>();
    s.days = j.at("days").get<std::array<Day, 30>>();
}

#endif // HASHTABLE_SHARE_H
