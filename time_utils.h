//
// Created by vik on 24.03.18.
//

#ifndef HASHTABLE_TIME_UTILS_H
#define HASHTABLE_TIME_UTILS_H

#include <cstring>
#include <iostream>
#include <ctime>


/**
 *
 * @param ts std::time_t (timestamp)
 * @return string representing the timestamp
 */
std::string ts_toDate(std::time_t ts)
{
    tm* tm = gmtime(&ts);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", tm);
    return std::string(date);
}

/**
 *
 * @param date a string in the format %Y-%m-%d
 * @return std::time_t timestamp
 */
time_t make_ts(std::string& date)
{
    struct tm tm{};

    memset(&tm, 0, sizeof(struct tm)); //clean all fields
    if (strptime(date.c_str(), "%Y-%m-%d", &tm) != nullptr) {
        time_t ts;
        ts = timegm(&tm);
        if (ts != -1) {
            return ts;
        }
    }
    return 0;
}

#endif //HASHTABLE_TIME_UTILS_H
