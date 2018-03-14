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
    std::array<Day, 30> *days; //FIXME soll pointer sein wegen cash line vorteil (cash locality)  weil in der regel 64 byte gelesen auf einmal und dann hab ich mehr shares vllt schon gelesen, wenn ich days direkt drin hab les ich days mit
public:
    Share(std::string& name, std::string& wkn, std::string& id)
            :name(name),
             wkn(wkn),
             id(id)
    {}

    //Getter Setter
    std::string get_name() const {
        return name;
    }

    std::string get_wkn() const {
        return wkn;
    }
};


#endif //HASHTABLE_SHARE_H
