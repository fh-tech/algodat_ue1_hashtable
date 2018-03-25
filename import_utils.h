//
// Created by vik on 25.03.18.
//

#ifndef HASHTABLE_IMPORT_UTILS_H
#define HASHTABLE_IMPORT_UTILS_H

#include <iostream>
#include <fstream>
#include <deque>
#include <algorithm>
#include "Day.h"
#include "Share.h"

// newest entries are at front now!
std::deque<std::string> parse_csv(std::ifstream &input_file) {
    std::deque<std::string> lines;
    while(!input_file.eof()) {
        std::string line;
        getline(input_file, line);
        if(!line.empty()) {
            std::replace(line.begin(), line.end(), ',', ' ');
            lines.push_front(line);
        }
    }
    lines.pop_back(); // remove header line
    return lines;
}

// file needs to have newest entrys last!
std::array<Day, 30> import_fromFile(std::ifstream &input_file) {
    std::deque<std::string> lines = parse_csv(input_file);
    std::array<Day, 30> days{};
    // else only header line
    if(lines.size() > 0) {
        int line_count = 0;
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

void updateImport(Share* share) {
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

                //update logic
                // would have been nice but fails if for example
                // actual = 3, 15, 18,...
                // new =   10, 11, 12, 13, 14
                int i = 0;
                for(int i = 0; i < 30 ; i++) {
                    if(days_new[i].date > days_actual[i].date) break;
                }
                memcpy(&days_actual[i], &days_new[i], (30 - i) * sizeof(Day));


                //REVIEW: more complicated but works even in edge cases like above mentioned
                /*for(int i = 0; i < 30; i++) {
                    if(days_new[i].date > days_actual[i].date) days_actual[i] = days_new[i];
                    for(int j = 0; j < i+1; j++) {
                        if(days_new[j].date > days_actual[i].date && days_new[j].date < days_actual[i+1].date) days_actual[i] = days_new[j];
                    }
                }*/
                std::cout << "Import was successful" << std::endl;
            } else {
                std::cout << "operation aborted" << std::endl;
                input_file.close();     //dont forget to close in either case
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
void import() {
    Share *share;
    std::string name;
    std::string id; //kürzel
    std::string contin;

    std::cout << "Please press (1) to update share information or (2) to make new share with the data: ";
    std::cin >> contin;
    if (contin == "1") {
        share = search();
        if(!share) {
            std::cout << "No share with specified information found" << std::endl;
            return;
        }
    } else if(contin == "2") {
        share = add();
    } else {
        std::cout << "No valid input. Aborting." << std::endl;
    }
    if(!share) {
        return;
    }
    //at this point its sure that the share * is valid now we can do updateImport
    updateImport(share);
}




#endif //HASHTABLE_IMPORT_UTILS_H
