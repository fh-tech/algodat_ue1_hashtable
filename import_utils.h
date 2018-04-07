//
// Created by vik on 25.03.18.
//

#ifndef HASHTABLE_IMPORT_UTILS_H
#define HASHTABLE_IMPORT_UTILS_H

#include "Day.h"
#include "Share.h"
#include "main_utils.h"
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>

std::deque<std::string> parse_csv(std::ifstream& input_file);
std::array<Day, 30> import_fromFile(std::ifstream& input_file);
void updateDays(std::array<Day, 30>& target, std::array<Day, 30>& source);
void updateImport(Share* share);
void import();

// newest entries are at front now!
std::deque<std::string> parse_csv(std::ifstream& input_file)
{
    std::deque<std::string> lines;
    while (!input_file.eof()) {
        std::string line;
        getline(input_file, line);
        if (!line.empty()) {
            std::replace(line.begin(), line.end(), ',', ' ');
            lines.push_front(line);
        }
    }
    lines.pop_back(); // remove header line
    return lines;
}

// file needs to have newest entrys last!
std::array<Day, 30> import_fromFile(std::ifstream& input_file)
{
    std::deque<std::string> lines = parse_csv(input_file);
    std::array<Day, 30> days{};
    // else only header line
    if (lines.empty()) {
        size_t line_count = 0;
        while (line_count < 30 && line_count < lines.size()) {
            std::string line = lines.at(line_count);
            std::stringstream line_stream(line);
            //REVIEW: maybe read first line and check if identical ?
            // line.empty important otherwise unnecessary iteration if newline at end
            if (!line.empty() && line_count < 30) {
                //REVIEW: i thought when i make a new variable in loop it is not related to last in any way and is initialized by default with default constructor...
                std::string date{}; // always initialize with default values so on fail when filling no values again
                float open{};
                float high{};
                float low{};
                float close{};
                float adj_close{};
                uint32_t volume{};
                //fill all vars from stream;
                line_stream >> date >> open >> high >> low >> close >> adj_close >> volume;
                time_t unix_date = make_ts(date);
                days[line_count] = Day(unix_date, open, high, low, close, adj_close, volume);
            }
            line_count++;
        }
    } else {
        std::cout << "Nothing to import." << std::endl;
    }
    input_file.close();
    return days;
}

//update logic
void updateDays(std::array<Day, 30>& target, std::array<Day, 30>& source)
{
    std::array<Day, 30> newDays{};
    int i,j,k;
    i = j = k = 0;

    while(k < 30) {
        // <= otherwise endless in some cases
        // avoid duplicate days
        if(target[i].date == source[j].date) {
            int start = j;
            while(target[i].date == source[j].date && (k+(j-start)) < 30) {
                i++;
                j++;
            }
            int count = (j-start);
            memcpy(&newDays[k], &source[start], count * sizeof(Day));
            k+=count;
        } else if(target[i] < source[j]) {
            int start = j;
            //do it as long as its bigger and you want copy past the new array
            while(target[i] < source[j] && (k+(j-start)) < 30) j++;

            int count = (j-start);

            memcpy(&newDays[k], &source[start], count * sizeof(Day));
            //add to counter how many elements were just added
            k += count;
        } else if(target[i] > source[j]) {
            int start = i;
            while(target[i] > source[j] && (k+(i-start)) < 30) i++;
            int count = (i-start);
            memcpy(&newDays[k], &target[start], count * sizeof(Day));
            k += count;
        }
    }
    target = newDays;
}

//another possibility but can not handle duplicates (would be even slower) --> would be more practical with a vector instead of array
void updateDays2(std::array<Day, 30>& target, std::array<Day,30>& source) {
    std::array<Day, 60> tmp{};
    memcpy(&tmp[0], &target[0], 30 * sizeof(Day));
    memcpy(&tmp[30], &source[0], 30 * sizeof(Day));

    // descending sort
    std::sort(tmp.rbegin(), tmp.rend());
    std::unique(tmp.begin(), tmp.end(), [](Day& d1, Day& d2){ return d1.date == d2.date;});
    // copy newest 30 into target
    memcpy(&target[0], &tmp[0], 30 * sizeof(Day));
}

void updateImport(Share* share)
{
    std::string contin;
    if (share) {
        std::cout << "Share found. Continue importing? (y/n): ";
        std::cin >> contin;
        if (contin == "y") {
            std::ifstream input_file;
            do {
                std::string pathToFile;
                std::cout << "Please enter the absolute path of the course data you want to import: ";
                std::cin >> pathToFile;
                input_file = std::ifstream(pathToFile);
            } while (!input_file);
            std::cout << "Found file. Start import? (y/n): ";
            std::cin >> contin;
            if (contin == "y") {
                std::cout << "importing please wait...";
                std::array<Day, 30> days_new = import_fromFile(input_file);
                std::array<Day, 30>& days_actual = share->days;

                updateDays(days_actual, days_new);

                std::cout << "Import was successful" << std::endl;
            } else {
                std::cout << "operation aborted" << std::endl;
                input_file.close(); //dont forget to close in either case
            }
        } else {
            std::cout << "Operation aborted." << std::endl;
            return;
        }
    } else {
        std::cout << "No share with specified information found." << std::endl;
        return;
    }
}

// returns new Share on success nullptr on failure
void import()
{
    Share* share{};
    std::string name;
    std::string id; //kürzel
    std::string contin;

    std::cout << "Please press (1) to update share information or (2) to make new share with the data: ";
    std::cin >> contin;
    if (contin == "1") {
        share = search();
        if (!share) {
            std::cout << "No share with specified information found" << std::endl;
            return;
        }
    } else if (contin == "2") {
        share = add();
    } else {
        std::cout << "No valid input. Aborting." << std::endl;
    }
    if (!share) {
        return;
    }
    //at this point its sure that the share * is valid now we can do updateImport
    updateImport(share);
}

#endif //HASHTABLE_IMPORT_UTILS_H
