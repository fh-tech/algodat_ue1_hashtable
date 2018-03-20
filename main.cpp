#include <algorithm>
#include <cstring>
#include <fstream>
#include "hashmap.h"

std::string import() {}

static HashTable hashTable(2048);

void plot() {
    std::array<std::array<char, 30>, 20> plot{};
    std::memset(plot.begin(), ' ', plot.size() * plot[0].size());

    Share s = {"Microsoft", "MSFT", "1245678"};

    int i = 0;
    for (auto &day: s.days) {
        day.close = i++ * i;
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

static std::array<std::string, 8> CommandStr =
        {"add", "delete", "import", "plot", "save", "load", "quit", "search"};

enum Command {
    ADD = 0,
    DELETE = 1,
    IMPORT = 2,
    PLOT = 3,
    SAVE = 4,
    LOAD = 5,
    QUIT = 6,
    SEARCH_NAME = 7,
    SEARCH_KEY = 8,
    INVALID = -1
};

void parse_input(const std::string &input) {
    for (auto i = 0; i < CommandStr.size(); i++) {
        if (CommandStr[i] == input) {
            auto c = static_cast<Command>(i);
            switch (c) {
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
                case SAVE:  save();
                    return;
                case LOAD:  load();
                    return;

                case QUIT:
                    exit(0);

                case SEARCH_NAME: //search_name();
                    return;
                case SEARCH_KEY: //search_id();
                    return;
                default:
                    exit(1);
            }
        }
    }
    std::cout << "Wrong input. Try again." << std::endl;

}

int main() {
    std::cout <<
              "============= Available Actions ================="
                      "\n"
                      "ADD: Manually input new Stock\n"
                      "DEL: Remove a stock by ID or name\n"
                      "IMPORT: Import stock information form a .csv file\n"
                      "SEARCH: Search information for a specific stock either by name or ID\n"
                      "PLOT: Visualize stock data\n"
                      "SAVE: Save current program data to json file\n"
                      "LOAD: Load data from a json file\n"
                      "QUIT: Exits the program. Make sure to save before." << std::endl;
    while (true) {
        std::cout << " > " << std::flush;

        std::string user_input;
        std::cin >> user_input;

        parse_input(user_input);
    }
}
