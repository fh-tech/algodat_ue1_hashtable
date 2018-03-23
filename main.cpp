#include <algorithm>
#include <cstring>
#include <fstream>
#include "hashmap.h"

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

//needs to be in format YYYY-MM-DD
uint32_t make_ts(std::string &date) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));     //clean all fields
    if (strptime(date.c_str(), "%Y-%m-%d", &tm) != NULL) {
        time_t ts;
        ts = mktime(&tm);
        if (ts != -1) {
            return ts;
        }
    }
    return 0; //TODO: think about what to do if strptime/mktime fails
}

std::array<Day, 30> import_fromFile(std::ifstream &input_file) {
    std::array<Day, 30> days{};
    int line_count = -1;
    while (!input_file.eof() && line_count < 30) {
        std::string line;
        getline(input_file, line);
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream line_stream(line);
        //TODO: maybe read first line and check if identical ?
        if (line_count > -1 &&
            !line.empty()) { // line.empty important otherwise unnecessary iteration if newline at end
            //FIXME: i thought when i make a new variable in loop it is not related to last in any way and is initialized by default with default constructor...
            std::string date{}; // always initialize with default values so on fail when filling no values again
            float open{};
            float high{};
            float low{};
            float close{};
            float adj_close{};
            uint32_t volume{};
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

bool validInput(const std::string &id, const std::string &wkn, const std::string &name, bool verbose) {
    bool valid = !name.empty() && !id.empty() && !wkn.empty();
    if (!valid) std::cout << "No empty values allowed" << std::endl;
    return valid;
}

Share* add() {
    Share share{};
    std::string name;
    std::string wkn;
    std::string id; //kürzel
    std::string contin;
    Share* share_p;
    do {
        std::cout << "Please enter the name of the share: ";
        std::cin >> name;

        std::cout << "Please enter the WKN(Wertpapierkennnummer): ";
        std::cin >> wkn;

        std::cout << "Please enter the ID(Kürzel): ";
        std::cin >> id;
    } while (!validInput(id, wkn, name, true));
    std::cout << "A new share with the following details will be created: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "WKN: " << wkn << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Continue creating? (y/n): ";
    std::cin >> contin;
    if (contin == "y") {
        Share share(name, id, wkn);
        share_p = hashTable.insert(std::move(share)); //ownership to hashtable with move
    } else {
        std::cout << "Operation aborted.";
        return nullptr;
    }

    if(!share_p) {
        std::cout << "Inserting the share failed." << std::endl;
    } else {
        std::cout << "Successfully added the share." << std::endl;
    }
    return share_p;
}

template<HashTable::KeyType keytype>
Share* search(std::string& key) {
    Share * share;
    if constexpr (keytype) {
        share = hashTable.get_by_name(key);
    } else {
        share = hashTable.get_by_id(key);
    }
    // does work with when its nullptr?
    if(!share) {
        std::cout << "No share with specified information found." << std::endl;
    }
    return share;
}

void updateImport(Share* share) {
    std::string contin;
    //now either share found with name or id or nullptr
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
                std::array<Day, 30> days = import_fromFile(input_file);
                //think more about that maybe
                share->days = days;
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
        std::cout << "Press (1) to enter the name of the share (2) to enter id of the share: ";
        std::cin >> contin;
        if (contin == "1") {
            std::cout << "Enter name of share to update: ";
            std::cin >> name;
            // noch überlegen ob wrapper methode notwendig
            share = search<HashTable::KeyType::NAME>(name);
        } else if (contin == "2") {
            std::cout << "Enter id of share to update: ";
            std::cin >> id;
            share = search<HashTable::KeyType::ID>(id);
        } else {
            std::cout << "Not a valid option. Quitting.";
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
    ADD = 1,
    DELETE = 2,
    IMPORT = 3,
    SEARCH = 4,
    PLOT = 5,
    SAVE = 6,
    LOAD = 7,
    QUIT = 8,
    INVALID = -1
};

void parse_input(std::string &input) {
    int command;
    try {
        command = std::stoi(input);
        if (command < 1 || command > 8) command = -1;
    } catch (std::invalid_argument) {
        command = -1;
    }

    switch (command) {
        case ADD:    //add();
            add();
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
    while (true) {
        std::cout <<
                  "\n"
                  "============= Available Actions ================="
                          "\n"
                          "(1)ADD: Manually input new Stock\n"
                          "(2)DEL: Remove a stock by ID or name\n"
                          "(3)IMPORT: Import stock information form a .csv file\n"
                          "(4)SEARCH: Search information for a specific stock either by name or ID\n"
                          "(5)PLOT: Visualize stock data\n"
                          "(6)SAVE: Save current program data to json file\n"
                          "(7)LOAD: Load data from a json file\n"
                          "(8)QUIT: Exits the program. Make sure to save before." << std::endl;

        std::cout << " > " << std::flush;

        std::string user_input;
        std::cin >> user_input;

        parse_input(user_input);
    }
}

