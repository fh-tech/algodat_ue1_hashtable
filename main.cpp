#include "hashmap.h"
#include "main_utils.h"
#include <algorithm>
#include <cstring>
#include <deque>
#include <fstream>

int main()
{
    while (true) {
        std::cout << "\n"
                     "============= Available Actions ================="
                     "\n"
                     "(1)ADD: Manually input new Stock\n"
                     "(2)DEL: Remove a stock by ID or name\n"
                     "(3)IMPORT: Import stock information from a .csv file\n"
                     "(4)SEARCH: Search information for a specific stock either by name or ID\n"
                     "(5)PLOT: Visualize stock data\n"
                     "(6)SAVE: Save current program data to json file\n"
                     "(7)LOAD: Load data from a json file\n"
                     "(8)QUIT: Exits the program. Make sure to save before."
                  << std::endl;

        std::cout << " > " << std::flush;

        std::string user_input;
        std::cin >> user_input;

        parse_input(user_input);
    }
}
