#include <algorithm>
#include <cstring>
#include <fstream>
#include "hashmap.h"
#include "time_utils.h"

static HashTable hashTable{1'000};

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

Share* add();
void delete_share();
std::array<Day, 30> import_fromFile(std::ifstream &input_file);
void updateImport(Share* share);
void import();
Share* search();
void plot();
void save();
void load();
void parse_input(std::string &input);

int main() {
    while (true) {
        std::cout <<
                  "\n"
                  "============= Available Actions ================="
                          "\n"
                          "(1)ADD: Manually input new Stock\n"
                          "(2)DEL: Remove a stock by ID or name\n"
                          "(3)IMPORT: Import stock information from a .csv file\n"
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

void delete_share() {
    std::string contin;
    Share* share;
    share = search();
    if(!share) {
        std::cout << "No share with specified information found.";
    } else {
        std::cout << "Found share. Are you sure you want to delete? (y/n): ";
        std::cin >> contin;
        if(contin == "y") {
            hashTable.remove_by_id(share->id);
            std::cout << "Removed the share." << std::endl;
        } else {
            std::cout << "Operation aborted." << std::endl;
        }
    }
}

std::array<Day, 30> import_fromFile(std::ifstream &input_file) {
    std::array<Day, 30> days{};
    int line_count = -1;
    while (!input_file.eof() && line_count < 30) {
        std::string line;
        getline(input_file, line);
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream line_stream(line);
        //REVIEW: maybe read first line and check if identical ?
        // line.empty important otherwise unnecessary iteration if newline at end
        if (line_count > -1 && !line.empty() && line_count < 30) {
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

Share* search() {
    std::string contin;
    std::string name;
    std::string id;
    Share * share;
    std::cout << "Press (1) to enter the name of the share (2) to enter id of the share: ";
    std::cin >> contin;
    if (contin == "1") {
        std::cout << "Enter name of share: ";
        std::cin >> name;
        // noch überlegen ob wrapper methode notwendig
        share = hashTable.get_by_name(name);
    } else if (contin == "2") {
        std::cout << "Enter id of share: ";
        std::cin >> id;
        share = hashTable.get_by_id(id);
    } else {
        std::cout << "Not a valid option. Quitting." << std::endl;
        return nullptr;
    }
    return share;
}

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

void save() {
    std::cout << "Please enter an absolute file name: \n>";
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

//TODO: auslagern der methoden in helper implementation file maybe
//TODO: write own plot method i think daniels is wrong it plots something with empty days (max value)
//REVIEW the json save part daniel made
//TODO: write tests for everything too (think about some edge cases)
//FIXME: saving then loading then searching for name of share produces SIGSEGV


void parse_input(std::string &input) {
    int command;
    try {
        command = std::stoi(input);
        if (command < 1 || command > 8) command = -1;
    } catch (std::invalid_argument) {
        command = -1;
    }

    switch (command) {
        case ADD:
            add();
            return;
        case DELETE:
            delete_share();
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
        case SEARCH:
            Share *share;
            share = search();
            if (!share) {
                std::cout << "Share could not be found." << std::endl;
            } else {
                std::cout << "Share found." << std::endl;
                share->days[0].printDay();
            }
            return;
        case INVALID:
            std::cout << "Wrong input. Try again." << std::endl;
            return;
        default:
            exit(1);
    }
}






