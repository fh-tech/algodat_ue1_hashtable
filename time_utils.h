//
// Created by vik on 24.03.18.
//

#ifndef HASHTABLE_TIME_UTILS_H
#define HASHTABLE_TIME_UTILS_H

#include <iostream>
#include <time.h>
#include <cstring>

std::string ts_toDate(std::time_t ts) {
    tm *tm = gmtime(&ts);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", tm);
    return std::string(date);  //REVIEW: Oh boy he done did it again
}

time_t make_ts(std::string &date) {
    struct tm tm{}; //REVIEW: initalize then memset is useless
    memset(&tm, 0, sizeof(struct tm));     //clean all fields
    if (strptime(date.c_str(), "%Y-%m-%d", &tm) != nullptr) {
        time_t ts;
        ts = timegm(&tm);
        if (ts != -1) {
            return ts;
        }
    }
    return 0; //REVIEW: think about what to do if strptime/mktime fails
}



#endif //HASHTABLE_TIME_UTILS_H
