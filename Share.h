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

    Share(std::string&& name, std::string&& wkn, std::string&& id)
        : name(std::move(name))
        , wkn(std::move(wkn))
        , id(std::move(id))
    {
    }

    Share(Share&& other) noexcept          //means can not throw
        : wkn(std::move(other.wkn))
        , name(std::move(other.name))
        , id(std::move(other.id))
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
