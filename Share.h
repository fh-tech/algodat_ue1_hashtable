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
    // FIXME soll pointer sein wegen cash line vorteil - wegen quadratic probing aber unnötig geworden außer bei erster probing iteration
    // (cash locality)  weil in der regel 64 byte
    // gelesen auf einmal und dann hab ich mehr shares
    // vllt schon gelesen, wenn ich days direkt drin hab
    // les ich days mit

    Share() = default;

    Share(std::string&& name, std::string&& id, std::string&& wkn)
        : name(std::move(name))
        , wkn(std::move(wkn))
        , id(std::move(id))
    {
    }

    Share(Share&& other) noexcept          //means can not throw
        : id(std::move(other.id))
        , name(std::move(other.name))
        , wkn(std::move(other.wkn))
        , days(other.days)
    {
    }

    bool operator==(const Share& other) const
    {
        return name == other.name
            && id == other.id
            && wkn == other.wkn
            && days == other.days;
    }
};

#endif // HASHTABLE_SHARE_H
