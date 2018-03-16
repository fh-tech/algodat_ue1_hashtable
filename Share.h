//
// Created by vik on 11.03.18.
//

#ifndef HASHTABLE_SHARE_H
#define HASHTABLE_SHARE_H


#include <cstdint>
#include <iostream>
#include <array>
#include "Day.h"

class Share {
private:
    std::string name;
    std::string wkn; //wertpapierkennnummer
    std::string id; //kürzel zum beispiel MSFT für microsoft aktie
    std::array<Day, 30> days;
public:
    Share(std::string& name, std::string& wkn, std::string& id, std::array<Day, 30>& days)
            :name(name),
             wkn(wkn),
             id(id),
             days(days)
    {}
    Share() = default;

    //Getter Setter
    std::string get_name() const {
        return name;
    }

    std::string get_wkn() const {
        return wkn;
    }
};


#endif //HASHTABLE_SHARE_H
