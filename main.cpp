#include <algorithm>
#include <cstring>
#include <fstream>
#include "hashmap.h"

std::string import() {}

static HashTable hashTable{1'000};

void plot() {
    std::array<std::array<char, 30>, 20> plot{};
    std::memset(plot.begin(), ' ', plot.size() * plot[0].size());

    Share s = {"Microsoft", "MSFT", "1245678"};

    int j = 0;
    for (auto &day: s.days) {
        day.close = j++ * j;
    }

    auto[daymin, daymax] = std::minmax_element(std::begin(s.days), std::end(s.days),
                                               [](Day &d0, Day &d1) { return d0.close < d1.close; });

    std::cout << "min: " << daymin->close << '\n';
    std::cout << "max: " << daymax->close << '\n';

    float delta = daymax->close - daymin->close;
    float delta_row = delta / plot.size();

    for (int i = 0; i < s.days.size(); ++i) {
        Day &d = s.days[i];
        plot[static_cast<size_t >(19.0 - (d.close / delta_row))][i] = '#';
    }
    std::cout << s.name << "   :    " << s.id << " (" << s.wkn << ")\n";
    for (auto &row: plot) {
        std::cout << std::string(row.cbegin(), row.cend()) << std::endl;
    }
}

void load() {
    std::cout << "Please enter a absolute file name: \n>";
    std::string file_name;
    std::cin >> file_name;
    try {
        std::ifstream json_file(file_name);
        json j;

        json_file >> j;

        hashTable = j;

    } catch (...) {
        std::cout << "unable to read json file" << std::endl;
    }
}

void save() {
    std::cout << "Please enter a absolute file name: \n>";
    std::string file_name;
    std::cin >> file_name;
    try {
        std::ofstream json_file(file_name);

        json j = hashTable;

        json_file << j << std::flush;

    } catch (...) {
        std::cout << "unable to read json file" << std::endl;
    }
}


enum Command {
    ADD = 0,
    DELETE = 1,
    IMPORT = 2,
    SEARCH = 3,
    PLOT = 4,
    SAVE = 5,
    LOAD = 6,
    QUIT = 7,
    INVALID = -1
};

void parse_input(std::string &input) {
    int command;
    try {
        command = std::stoi(input);
        if (command < -1 || command > 7) command = -1;
    } catch (std::invalid_argument) {
        command = -1;
    }

    switch (command) {
        case ADD:    //add();
            return;
        case DELETE: //delete();
            return;
        case IMPORT:
            import();
            return;
        case PLOT:
            plot();
            return;
        case SAVE:
            save();
            return;
        case LOAD:
            load();
            return;
        case QUIT:
            std::cout << "Quitting program." << std::endl;
            exit(0);
        case SEARCH: //search_name(); decide on kind here
            return;
        case INVALID:
            std::cout << "Wrong input. Try again." << std::endl;
            return;
        default:
            exit(1);
    }
}

int main() {
    std::cout <<
              "============= Available Actions ================="
                      "\n"
                      "(1)ADD: Manually input new Stock\n"
                      "(2)DEL: Remove a stock by ID or name\n"
                      "(3)IMPORT: Import stock information form a .csv file\n"
                      "(3)SEARCH: Search information for a specific stock either by name or ID\n"
                      "(4)PLOT: Visualize stock data\n"
                      "(5)SAVE: Save current program data to json file\n"
                      "(6)LOAD: Load data from a json file\n"
                      "(7)QUIT: Exits the program. Make sure to save before." << std::endl;
    while (true) {
        std::cout << " > " << std::flush;

        std::string user_input;
        std::cin >> user_input;

        parse_input(user_input);
    }
}
