#include "hashmap.h"

std::string import() {}

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

void parse_input(const std::string& input) {
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
                case PLOT:  //plot();
                    return;
                case SAVE:  //save();
                    return;
                case LOAD:  //load();
                    return;

                case QUIT:  exit(0);

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
    while(true) {
        std::cout << " > " << std::flush;

        std::string user_input;
        std::cin >> user_input;

        parse_input(user_input);
    }
}
