//
// Created by vik on 24.03.18.
//

#ifndef HASHTABLE_TIME_UTILS_H
#define HASHTABLE_TIME_UTILS_H

#include <iostream>
#include <time.h>
#include <cstring>

std::string ts_toDate(time_t ts) {
    tm *tm = gmtime(&ts);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", tm);
    return date;
}

time_t make_ts(std::string &date) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));     //clean all fields
    if (strptime(date.c_str(), "%Y-%m-%d", &tm) != NULL) {
        time_t ts;
        ts = timegm(&tm);
        if (ts != -1) {
            return ts;
        }
    }
    return 0; //REVIEW: think about what to do if strptime/mktime fails
}



#endif //HASHTABLE_TIME_UTILS_H
