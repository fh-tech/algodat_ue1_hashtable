#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include "Hashtable.h"
#include "Day.h"


//needs to be in format YYYY-MM-DD
uint32_t make_ts(std::string& date) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));     //clean all fields
    if(strptime(date.c_str(), "%Y-%m-%d", &tm) != NULL) {
        time_t ts;
        ts = mktime(&tm);
        if (ts != -1) {
            return ts;
        }
    }
    return 0; //TODO: think about what to do if strptime/mktime fails
}

std::array<Day, 30> import_fromFile(std::ifstream& input_file) {
    std::array<Day, 30> days{};
    int line_count = -1;
    while (!input_file.eof() && line_count < 30) {
        std::string line;
        getline(input_file, line);
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream line_stream(line);
        //TODO: maybe read first line and check if identical ?
        if (line_count > -1 && !line.empty()) { // line.empty important otherwise unnecessary iteration if newline at end
            //FIXME: i thought when i make a new variable in loop it is not related to last in any way and is initialized by default with default constructor...
            std::string date {}; // always initialize with default values so on fail when filling no values again
            float open {};
            float high {};
            float low {};
            float close {};
            float adj_close {};
            uint32_t volume {};
            //fill all vars from stream;
            line_stream >> date >> open >> high >> low >> close >> adj_close >> volume;
            uint32_t unix_date = make_ts(date);
            days[line_count] = Day(unix_date, open, high, low, close, adj_close, volume);
        }
        line_count++;
    }
    input_file.close();
    return days;
}

// returns new Share on success nullptr on failure
Share import() {
    Share share{};
    std::string name;
    std::string wkn;
    std::string id; //kürzel
    std::string contin;

    std::cout << "Please enter the name of the share: ";
    std::cin >> name;

    std::cout << "Please enter the WKN(Wertpapierkennnummer): ";
    std::cin >> wkn;

    std::cout << "Please enter the ID(Kürzel): ";
    std::cin >> id;

    std::cout << "A new share with the following details will be created: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "WKN: " << wkn << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Continue creating/import data? (y/n): ";
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
            std::array<Day, 30> days = import_fromFile(input_file);
            share = Share(name, wkn, id, days);
            //do i need to close it here or is it good enough if i close the reference
        } else {
            std::cout << "operation aborted" << std::endl;
            input_file.close();     //dont forget to close in either case
        }
    } else {
        std::cout << "operation aborted" << std::endl;
    }
    std::cout << "Done!" << std::endl;
    return share;
}




// Questions for later or prof --> Constructors in general
// try Share share; --> will use random values in days array     (maybe does not use default constructor?) but only works if you have one!
// try Share share{} --> will use default constructor values in days array (also only works if you have a default constructor)


// try std::array<Day, 10> days;   --> days will be randomly initialized
// try std::array<Day, 10> days{}; --> days default initialized

// vector always default initialized

// Day d;
// Day d {};

int main() {

}
